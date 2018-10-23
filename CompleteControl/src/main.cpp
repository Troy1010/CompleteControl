#pragma region Settings
constexpr auto DebugThreshold = 4;
#pragma endregion
#pragma region Includes
#include "obse/PluginAPI.h"
#include "obse/CommandTable.h"
#include "obse_common/SafeWrite.cpp"
#if OBLIVION
#include "obse/GameAPI.h"
OBSECommandTableInterface * g_commandTableIntfc = NULL; // assigned in OBSEPlugin_Load
OBSEScriptInterface * g_scriptIntfc = NULL; //For command argument extraction
#define ExtractArgsEx(...) g_scriptIntfc->ExtractArgsEx(__VA_ARGS__)
#define ExtractFormatStringArgs(...) g_scriptIntfc->ExtractFormatStringArgs(__VA_ARGS__)
#else
#include "obse_editor/EditorAPI.h"
#endif
#include "obse/ParamInfos.h"
#include <vector>
#include <set>
#include "Control.h"
#include <string>
#include "TM_CommonCPP/Misc.h"
#include "TM_CommonCPP/Narrate.h"
#include "TM_CommonCPP_NarrateOverloads.h"
#include "obse/Script.h"
#include "obse/Hooks_DirectInput8Create.h"
#pragma endregion
#pragma region Globals
IDebugLog		gLog("CompleteControl.log"); //Log
Cmd_Execute DisableKey_OriginalExecute = NULL; //Execute of replaced DisableKey command
Cmd_Execute EnableKey_OriginalExecute = NULL; //Execute of replaced EnableKey command
const CommandInfo* DisableKey_CmdInfo; //DisableKey command
const CommandInfo* GetControl; //GetControl command
static std::vector<Control> Controls;

Script* pBlankScript = NULL;
ScriptEventList * pBlankScriptEventList = NULL;
bool bOblivionLoaded = false;
OBSESerializationInterface	* g_serialization = NULL;
PluginHandle				g_pluginHandle = kPluginHandle_Invalid;
#pragma endregion
#pragma region Macros
#define CCDebug(iLvl,sTxt) if (DebugThreshold >= iLvl) {FnDebug(sTxt);};
#pragma endregion
#pragma region CopyPasted
// Copy-pasted from obse's Control_Input
#define CONTROLSMAPPED 29
//Roundabout way of getting a pointer to the array containing control map
//Not sure what CtrlMapBaseAddr points to (no RTTI) so use brute pointer arithmetic
#if OBLIVION_VERSION == OBLIVION_VERSION_1_1
static const UInt32* CtrlMapBaseAddr = (UInt32*)0x00AEAAB8;
static const UInt32	 CtrlMapOffset = 0x00000020;
static const UInt32  CtrlMapOffset2 = 0x00001B7E;
#elif OBLIVION_VERSION == OBLIVION_VERSION_1_2
static const UInt32* CtrlMapBaseAddr = (UInt32*)0x00B33398;
static const UInt32	 CtrlMapOffset = 0x00000020;
static const UInt32  CtrlMapOffset2 = 0x00001B7E;
#elif OBLIVION_VERSION == OBLIVION_VERSION_1_2_416
static const UInt32* CtrlMapBaseAddr = (UInt32*)0x00B33398;
static const UInt32	 CtrlMapOffset = 0x00000020;
static const UInt32  CtrlMapOffset2 = 0x00001B7E;
#endif
UInt8*  InputControls = 0;
UInt8*  AltInputControls = 0;
static void GetControlMap()
{
	UInt32 addr = *CtrlMapBaseAddr + CtrlMapOffset;
	addr = *(UInt32*)addr + CtrlMapOffset2;
	InputControls = (UInt8*)addr;
	AltInputControls = InputControls + CONTROLSMAPPED;
}
static bool IsKeycodeValid(UInt32 id) { return id < kMaxMacros - 2; }
#pragma endregion
#pragma region HelperFunctions
//### SafeConsolePrint
void SafeConsolePrint(std::string sString)
{
	if (!bOblivionLoaded) // Trying to print to console without Oblivion loaded causes CTD.
	{
		//do nothing
	}
	else if (sString.length() < 1000) //1000 might not be the exact limit. Exceeding limit causes CTD.
	{
		Console_Print(sString.c_str());
	}
	else
	{
		Console_Print(sString.substr(0, 1000).c_str());
		Console_Print("<MessageTooLarge>");
	}
}
//### FnDebug
void FnDebug(std::string sString)
{
	SafeConsolePrint(sString);
	_MESSAGE(sString.c_str());
}
//### ExecuteCommand
auto ExecuteCommand(Cmd_Execute vCmdExecute, double vArg, COMMAND_ARGS)
{
	// Overload specific vars
	int iDataTypeCode = 0x7A; //double
	//
	double result2 = 0;
	UInt8* pData = new UInt8[3 + sizeof(double)];
	UInt16* vNumArgs = (UInt16*)pData;
	*vNumArgs = 1;
	pData[2] = iDataTypeCode;
	double* fArgsVal = (double*)&pData[3];
	*fArgsVal = vArg;
	UInt32 opOffsetPtr2 = 0;
	vCmdExecute(kParams_OneInt, pData, thisObj, arg3, scriptObj, eventList, &result2, &opOffsetPtr2);
	delete[] pData;
	return result2;
}
auto ExecuteCommand(Cmd_Execute vCmdExecute, double vArg)
{
	ParamInfo * paramInfo = NULL;
	void * arg1 = 0;
	TESObjectREFR * thisObj = NULL;
	UInt32 arg3 = 0;
	if (!pBlankScript)
	{
		CCDebug(6, "ExecuteCommand`pBlankScript INIT");
		pBlankScript = (Script*)CreateFormInstance(13);
		pBlankScriptEventList = (*pBlankScript).CreateEventList();
	}
	Script * scriptObj = pBlankScript;
	ScriptEventList * eventList = pBlankScriptEventList;
	double * result = 0;
	UInt32 * opcodeOffsetPtr = 0;
	return ExecuteCommand(vCmdExecute, vArg, PASS_COMMAND_ARGS);
}
auto ExecuteCommand(const CommandInfo* vCmd, double vArg, COMMAND_ARGS)
{
	return ExecuteCommand(vCmd->execute, vArg, PASS_COMMAND_ARGS);
}
auto ExecuteCommand(const CommandInfo* vCmd, double vArg)
{
	return ExecuteCommand(vCmd->execute, vArg);
}
//### InitializeControls
auto InitializeControls(std::vector<Control> &Controls)
{
	CCDebug(5,"InitializeControls`Open");
	//Register Controls
	for (int i = 0; i < Control::VanillaControlID_Count; ++i)
	{
		Controls.push_back(Control(ExecuteCommand(GetControl, i), i));
	}
	CCDebug(5, "InitializeControls`Controls:" + TMC::Narrate(Controls)); //verbose
	CCDebug(5,"InitializeControls`Close");
}
#pragma endregion
#pragma region CompleteControlAPI
//### CommandTemplate
bool Cmd_CommandTemplate_Execute(COMMAND_ARGS)
{
	CCDebug(5,"CommandTemplate`Open");
	CCDebug(5,"CommandTemplate`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(CommandTemplate, "CommandTemplate command", 0, 0, NULL)
//### HandleOblivionLoaded
bool Cmd_HandleOblivionLoaded_Execute(COMMAND_ARGS)
{
	CCDebug(5,"HandleOblivionLoaded`Open");
	bOblivionLoaded = true;
	InitializeControls(Controls);
	CCDebug(5,"HandleOblivionLoaded`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(HandleOblivionLoaded, "HandleOblivionLoaded command", 0, 0, NULL)
//### DisableKey_Replacing
bool Cmd_DisableKey_Replacing_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, UInt32 arg3, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
{
	CCDebug(5,"Cmd_DisableKey_Replacing_Execute`Open");
	UInt32	dxScancode = 0;
	UInt8	iModIndex = 0;
	//---Register in Controls
	//-Get dxScancode
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &dxScancode)) {
		CCDebug(5,"Cmd_DisableKey_Replacing_Execute`Failed arg extraction");
		return false;
	}
	//-Get iModIndex
	iModIndex = (UInt8)(scriptObj->refID >> 24);
	//-Register iModIndex in vControl.cModIndices
	for (Control &vControl : Controls)
	{
		if (vControl.dxScancode == dxScancode)
		{
			vControl.cModIndices_Disables.insert(iModIndex);
			break;
		}
	}
	//---DisableKey
	//-Execute Original DisableKey
	DisableKey_OriginalExecute(PASS_COMMAND_ARGS);
	CCDebug(5,"Cmd_DisableKey_Replacing_Execute`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(DisableKey_Replacing, "Disables a key and registers a disable", 0, 1, kParams_OneInt)
//### EnableKey_Replacing
bool Cmd_EnableKey_Replacing_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, UInt32 arg3, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
{
	CCDebug(5,"Cmd_EnableKey_Replacing_Execute`Open");
	UInt32	dxScancode = 0;
	UInt8	iModIndex = 0;
	//---Register in Controls
	//-Get dxScancode
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &dxScancode)) {
		CCDebug(5,"Cmd_EnableKey_Replacing_Execute`Failed arg extraction");
		return false;
	}
	//-Get iModIndex
	iModIndex = (UInt8)(scriptObj->refID >> 24);
	//-Unregister disable. Determine bDoEnableKey by checking if any disables are registered for our dxScancode
	bool bDoEnableKey = true;
	for (Control &vControl : Controls)
	{
		if (vControl.dxScancode == dxScancode)
		{
			vControl.cModIndices_Disables.erase(iModIndex);
			if (!(vControl.cModIndices_Disables.empty()))
			{
				bDoEnableKey = false;
			}
			break;
		}
	}
	//---Execute original EnableKey
	if (bDoEnableKey) {
		CCDebug(5, "Cmd_EnableKey_Replacing_Execute`EnablingKey");
		EnableKey_OriginalExecute(PASS_COMMAND_ARGS);
	}
	else
	{
		CCDebug(5, "Cmd_EnableKey_Replacing_Execute`Neglecting to enable key.");
	}
	CCDebug(5,"Cmd_EnableKey_Replacing_Execute`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(EnableKey_Replacing, "Unregisters disable of this mod. Enables key if there are no disables registered.", 0, 1, kParams_OneInt)
#pragma endregion
#pragma region SerializationHandlers
std::string	g_strData;
static void ResetData(void)
{
	g_strData.clear();
}

static void Handler_Save(void * reserved)
{
	// write out the string
	g_serialization->OpenRecord('STR ', 0);
	g_serialization->WriteRecordData(g_strData.c_str(), g_strData.length());

	// write out some other data
	g_serialization->WriteRecord('ASDF', 1234, "hello world", 11);
}

static void Handler_Load(void * reserved)
{
	UInt32	type, version, length;

	ResetData();

	char	buf[512];

	while (g_serialization->GetNextRecordInfo(&type, &version, &length))
	{
		CCDebug(4, "record %08X (%.4s) %08X %08X", type, &type, version, length);
		_MESSAGE("record %08X (%.4s) %08X %08X", type, &type, version, length);

		switch (type)
		{
		case 'STR ':
			g_serialization->ReadRecordData(buf, length);
			buf[length] = 0;

			CCDebug(4, "got string %s", buf);
			_MESSAGE("got string %s", buf);

			g_strData = buf;
			break;

		case 'ASDF':
			g_serialization->ReadRecordData(buf, length);
			buf[length] = 0;

			CCDebug(4, "ASDF chunk = %s", buf);
			_MESSAGE("ASDF chunk = %s", buf);
			break;
		default:
			_MESSAGE("Unknown chunk type $08X", type);
		}
	}
}

static void Handler_Preload(void * reserved)
{
	_MESSAGE("Preload Callback start");
	Handler_Load(reserved);
	_MESSAGE("Preload Callback finished");
}

static void Handler_NewGame(void * reserved)
{
	ResetData();
}
#pragma endregion
#pragma region Tests
//### PrintControls
bool Cmd_PrintControls_Execute(COMMAND_ARGS)
{
	CCDebug(5, "PrintControls`Controls:" + TMC::Narrate(Controls));
	return true;
}
DEFINE_COMMAND_PLUGIN(PrintControls, "PrintControls command", 0, 0, NULL)
//### TestCeil
bool Cmd_TestCeil_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, UInt32 arg3, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
{
	CCDebug(5, "TestCeil`Open");
	*result = 0;
	UInt8* fArgs = new UInt8[3 + sizeof(double)];
	UInt16* fArgsNumArgs = (UInt16*)fArgs;
	*fArgsNumArgs = 1;
	fArgs[2] = 0x7A; // argument type double
	double* fArgsVal = (double*)&fArgs[3];
	*fArgsVal = 18.42;
	UInt32 opOffsetPtr = 0;
	const CommandInfo* ceil = g_commandTableIntfc->GetByName("Ceil");
	ceil->execute(kParams_OneFloat, fArgs, thisObj, arg3, scriptObj, eventList, result, &opOffsetPtr);
	delete[] fArgs;
	CCDebug(5, "TestCeil`opcode:" + TMC::Narrate(ceil->opcode) + " *result:" + TMC::Narrate(*result) + " result:" + TMC::Narrate(result));
	CCDebug(5, "TestCeil`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(TestCeil, "TestCeil command", 0, 0, NULL)
//### BasicRuntimeTests
bool Cmd_BasicRuntimeTests_Execute(COMMAND_ARGS)
{
	CCDebug(5, "BasicRuntimeTests`Open");
	//*result = 0; //Do I need this?
	int iInt = 5;
	CCDebug(5, "5:" + TMC::Narrate(iInt));
	UInt8 vUInt8 = 3;
	CCDebug(5, "3:" + TMC::Narrate(vUInt8));
	std::set<UInt8> cSet;
	cSet.insert(65);
	cSet.insert(64);
	cSet.insert(63);
	CCDebug(5, "Set:" + TMC::Narrate(cSet));
	CCDebug(5, "ActualControls:" + TMC::Narrate(Controls));
	//static std::vector<Control> Controls_Fake;
	//Controls_Fake.push_back(Control(15,UInt32(4)));
	//for (Control &vControl : Controls_Fake)
	//{
	//	vControl.cModIndices_Disables.insert(222);
	//}
	//CCDebug(5,"FakeControls:" + TMC::Narrate(Controls_Fake));
	CCDebug(5, "BasicRuntimeTests`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(BasicRuntimeTests, "BasicRuntimeTests command", 0, 0, NULL)
//### TestGetControlDirectly
bool Cmd_TestGetControlDirectly_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, UInt32 arg3, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
{
	CCDebug(5, "TestGetControlDirectly`Open");
	double endResult;
	endResult = ExecuteCommand(GetControl, 2, PASS_COMMAND_ARGS);
	// Report
	//CCDebug(5,"TestGetControlDirectly`opcode:" + TMC::Narrate(GetControl->opcode) + " *result:" + TMC::Narrate(*result) + " result:" + TMC::Narrate(result));
	CCDebug(5, "TestGetControlDirectly`endResult:" + TMC::Narrate(endResult));
	CCDebug(5, "TestGetControlDirectly`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(TestGetControlDirectly, "TestGetControlDirectly command", 0, 0, NULL)
//### TestGetControlDirectly2
bool Cmd_TestGetControlDirectly2_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, UInt32 arg3, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
{
	CCDebug(5, "TestGetControlDirectly2`Open");
	double endResult;
	endResult = ExecuteCommand(GetControl, 2);
	// Report
	//CCDebug(5,"TestGetControlDirectly2`opcode:" + TMC::Narrate(GetControl->opcode) + " *result:" + TMC::Narrate(*result) + " result:" + TMC::Narrate(result));
	CCDebug(5, "TestGetControlDirectly2`endResult:" + TMC::Narrate(endResult));
	CCDebug(5, "TestGetControlDirectly2`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(TestGetControlDirectly2, "TestGetControlDirectly2 command", 0, 0, NULL)
//### TestGetControlCopyPasta
bool Cmd_TestGetControlCopyPasta_Execute(COMMAND_ARGS)
{
	CCDebug(5, "TestGetControlCopyPasta`Open");
	*result = 0xFFFF;
	UInt32	keycode = 0;
	//ExtractArgs
	if (!ExtractArgs(paramInfo, arg1, opcodeOffsetPtr, thisObj, arg3, scriptObj, eventList, &keycode)) return true;
	//
	if (!InputControls) GetControlMap();
	*result = InputControls[keycode];
	CCDebug(5, "TestGetControlCopyPasta`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(TestGetControlCopyPasta, "TestGetControlCopyPasta command", 0, 1, kParams_OneInt)
//### TestDisableKeyCopyPasta
bool Cmd_TestDisableKeyCopyPasta_Execute(COMMAND_ARGS)
{
	//Open
	*result = 0;
	UInt32	keycode = 0;
	CCDebug(5, "TestDisableKeyCopyPasta`Open");
	//
	if (!ExtractArgs(paramInfo, arg1, opcodeOffsetPtr, thisObj, arg3, scriptObj, eventList, &keycode)) return true;
	if (keycode % 256 == 255 && keycode < 2048) keycode = 255 + (keycode + 1) / 256;
	if (IsKeycodeValid(keycode)) DI_data.DisallowStates[keycode] = 0x00;
	CCDebug(5, "TestDisableKeyCopyPasta`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(TestDisableKeyCopyPasta, "TestDisableKeyCopyPasta command", 0, 0, NULL)
//### GenerateEnum
bool Cmd_GenerateEnum_Execute(COMMAND_ARGS)
{
	CCDebug(5, "GenerateEnum`Open");
	//Extract Args
	TESForm* rTemp = NULL;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &rTemp)) {
		CCDebug(5, "Cmd_DisableKey_Replacing_Execute`Failed arg extraction");
		return false;
	}
	//Report
	CCDebug(5, "rTemp:" + TMC::Narrate(rTemp->refID));
	//Return
	*result = rTemp->refID;
	CCDebug(5, "GenerateEnum`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(GenerateEnum, "GenerateEnum command", 0, 1, kParams_OneObjectRef)
#pragma endregion
#pragma region LoadEvent
extern "C" {
bool OBSEPlugin_Query(const OBSEInterface * obse, PluginInfo * info)
{
	CCDebug(5,"Query`Open");
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "CompleteControl";
	info->version = 1;
	// version checks
	if(!obse->isEditor)
	{
#if OBLIVION
		if(obse->oblivionVersion != OBLIVION_VERSION)
		{
			_ERROR("incorrect Oblivion version (got %08X need %08X)", obse->oblivionVersion, OBLIVION_VERSION);
			return false;
		}
#endif	
	}

	if (obse->obseVersion < OBSE_VERSION_INTEGER)
	{
		_MESSAGE("OBSE version too old (got %08X expected at least %08X)", obse->obseVersion, OBSE_VERSION_INTEGER);
		_MESSAGE("OBSE version too old (got %i expected at least %i)", obse->obseVersion, OBSE_VERSION_INTEGER);
		//return false;
	}

	CCDebug(5, "Query`Close");
	return true;
}
bool OBSEPlugin_Load(const OBSEInterface * obse)
{
	CCDebug(4, "Load`Open");
	g_pluginHandle = obse->GetPluginHandle();
	if (!obse->isEditor)
	{
		g_serialization = (OBSESerializationInterface *)obse->QueryInterface(kInterface_Serialization);
		if (!g_serialization)
		{
			_ERROR("serialization interface not found");
			return false;
		}
		if (g_serialization->version < OBSESerializationInterface::kVersion)
		{
			_ERROR("incorrect serialization version found (got %08X need %08X)", g_serialization->version, OBSESerializationInterface::kVersion);
			return false;
		}
		g_serialization->SetSaveCallback(g_pluginHandle, Handler_Save);
		g_serialization->SetLoadCallback(g_pluginHandle, Handler_Load);
		g_serialization->SetNewGameCallback(g_pluginHandle, Handler_NewGame);
	}
	obse->SetOpcodeBase(0x28B0);
	obse->RegisterCommand(&kCommandInfo_BasicRuntimeTests);
	obse->RegisterCommand(&kCommandInfo_TestGetControlDirectly);
	obse->RegisterCommand(&kCommandInfo_TestGetControlDirectly2);
	obse->RegisterCommand(&kCommandInfo_GenerateEnum);
	obse->RegisterCommand(&kCommandInfo_CommandTemplate);
	obse->RegisterCommand(&kCommandInfo_TestGetControlCopyPasta);
	obse->RegisterCommand(&kCommandInfo_TestDisableKeyCopyPasta);
	obse->RegisterCommand(&kCommandInfo_TestCeil);
	obse->RegisterCommand(&kCommandInfo_HandleOblivionLoaded);
	obse->RegisterCommand(&kCommandInfo_PrintControls);

	if (!obse->isEditor)
	{
		// get an OBSEScriptInterface to use for argument extraction
		g_scriptIntfc = (OBSEScriptInterface*)obse->QueryInterface(kInterface_Script);
		g_commandTableIntfc = (OBSECommandTableInterface*)obse->QueryInterface(kInterface_CommandTable);
		// replace DisableKey
		DisableKey_OriginalExecute = g_commandTableIntfc->GetByOpcode(0x1430)->execute; //DisableKey_opcode:00001430
		g_commandTableIntfc->Replace(0x1430, &kCommandInfo_DisableKey_Replacing);
		// replace EnableKey
		EnableKey_OriginalExecute = g_commandTableIntfc->GetByOpcode(0x1431)->execute; //EnableKey_opcode:00001431
		g_commandTableIntfc->Replace(0x1431, &kCommandInfo_EnableKey_Replacing);
		// Get GetControl
		GetControl = g_commandTableIntfc->GetByName("GetControl");
		//GetControl->
		//
		DisableKey_CmdInfo = g_commandTableIntfc->GetByOpcode(0x1430);
		//DisableKey_CmdInfo->execute();
	}

	CCDebug(4, "Load`Close");
	return true;
}
};
#pragma endregion
