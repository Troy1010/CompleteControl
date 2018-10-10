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
#define CC_Debug 1
#include "obse/Script.h"
#include "obse/Hooks_DirectInput8Create.h"

IDebugLog		gLog("CompleteControl.log"); //Log
Cmd_Execute DisableKey_OriginalExecute = NULL; //Execute of replaced DisableKey command
Cmd_Execute EnableKey_OriginalExecute = NULL; //Execute of replaced EnableKey command
const CommandInfo* DisableKey_CmdInfo; //DisableKey command
const CommandInfo* GetControl; //GetControl command
static std::vector<Control> Controls;

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

#pragma region HelperFunctions
void Debug_CC(std::string sString)
{
#if CC_Debug
	Console_Print(sString.c_str());
	_MESSAGE(sString.c_str());
#endif
}
#pragma endregion
#pragma region CompleteControlAPI
//### CommandTemplate
bool Cmd_CommandTemplate_Execute(COMMAND_ARGS)
{
	//Open
#if CC_Debug
	Debug_CC("CommandTemplate`Open");
#endif
	//
	//Close
#if CC_Debug
	Debug_CC("CommandTemplate`Close");
#endif
	return true;
}
DEFINE_COMMAND_PLUGIN(CommandTemplate, "CommandTemplate command", 0, 0, NULL)
//### TestCeil
bool Cmd_TestCeil_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, UInt32 arg3, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
{
	//Open
#if CC_Debug
	Debug_CC("TestCeil`Open");
#endif
	//
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
	Debug_CC("TestCeil`opcode:" + TM_CommonCPP::Narrate(ceil->opcode) + " *result:" + TM_CommonCPP::Narrate(*result) + " result:" + TM_CommonCPP::Narrate(result));
	//Close
#if CC_Debug
	Debug_CC("TestCeil`Close");
#endif
	return true;
}
DEFINE_COMMAND_PLUGIN(TestCeil, "TestCeil command", 0, 0, NULL)
//### BasicRuntimeTests
bool Cmd_BasicRuntimeTests_Execute(COMMAND_ARGS)
{
	//Open
#if CC_Debug
	Debug_CC("BasicRuntimeTests`Open");
	//*result = 0; //Do I need this?
	int iInt = 5;
	Debug_CC("5:" + TM_CommonCPP::Narrate(iInt));
	UInt8 vUInt8 = 3;
	Debug_CC("3:" + TM_CommonCPP::Narrate(vUInt8));
	std::set<UInt8> cSet;
	cSet.insert(65);
	cSet.insert(64);
	cSet.insert(63);
	Debug_CC("Set:" + TM_CommonCPP::Narrate(cSet));
	//Debug_CC("ActualControls:" + TM_CommonCPP::Narrate(Controls));
	//static std::vector<Control> Controls_Fake;
	//Controls_Fake.push_back(Control(15,UInt32(4)));
	//for (Control &vControl : Controls_Fake)
	//{
	//	vControl.cModIndices_Disables.insert(222);
	//}
	////Close
	//Debug_CC("FakeControls:" + TM_CommonCPP::Narrate(Controls_Fake));
	Debug_CC("BasicRuntimeTests`Close");
#endif
	return true;
}
DEFINE_COMMAND_PLUGIN(BasicRuntimeTests, "BasicRuntimeTests command", 0, 0, NULL)
//### TestGetControlDirectly
bool Cmd_TestGetControlDirectly_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, UInt32 arg3, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
{
	//Open
#if CC_Debug
	Debug_CC("TestGetControlDirectly`Open");
#endif
	//Extras
	*result = 0; //Do I need this?
	//
	//double vControlID = 0.0;
	//GetControl->execute(PASS_COMMAND_ARGS, 16);
	//UInt8* fArgs = new UInt8[3 + sizeof(double)];
	//UInt16* fArgsNumArgs = (UInt16*)fArgs;
	//*fArgsNumArgs = 1;
	//fArgs[2] = 0x7A; // argument type double
	//double* fArgsVal = (double*)&fArgs[3];
	//*fArgsVal = 2;
	//UInt32 opOffsetPtr = 0;
	////const CommandInfo* ceil = g_commandTableIntfc->GetByName("Ceil");
	////ceil->execute(kParams_OneInt, fArgs, thisObj, arg3, scriptObj, eventList, result, &opOffsetPtr);
	//GetControl->execute(kParams_OneFloat, fArgs, thisObj, arg3, scriptObj, eventList, result, &opOffsetPtr);
	//delete[] fArgs;
	//
	UInt8* fArgs = new UInt8[3 + sizeof(double)];
	UInt16* fArgsNumArgs = (UInt16*)fArgs;
	*fArgsNumArgs = 1;
	fArgs[2] = 0x7A; // argument type double
	double* fArgsVal = (double*)&fArgs[3];
	*fArgsVal = 2.0;
	UInt32 opOffsetPtr = 0;
	GetControl->execute(kParams_OneInt, fArgs, thisObj, arg3, scriptObj, eventList, result, &opOffsetPtr);
	delete[] fArgs;
	Debug_CC("TestGetControlDirectly`opcode:" + TM_CommonCPP::Narrate(GetControl->opcode) + " *result:" + TM_CommonCPP::Narrate(*result) + " result:" + TM_CommonCPP::Narrate(result));
	//Close
#if CC_Debug
	Debug_CC("TestGetControlDirectly`Close");
#endif
	return true;
}
DEFINE_COMMAND_PLUGIN(TestGetControlDirectly, "TestGetControlDirectly command", 0, 0, NULL)
//### TestGetControlCopyPasta
bool Cmd_TestGetControlCopyPasta_Execute(COMMAND_ARGS)
{
	//Open
#if CC_Debug
	Debug_CC("TestGetControlCopyPasta`Open");
#endif
	*result = 0xFFFF;
	UInt32	keycode = 0;
	//ExtractArgs
	if (!ExtractArgs(paramInfo, arg1, opcodeOffsetPtr, thisObj, arg3, scriptObj, eventList, &keycode)) return true;
	//
	if (!InputControls) GetControlMap();
	*result = InputControls[keycode];
	//Close
#if CC_Debug
	Debug_CC("TestGetControlCopyPasta`Close");
#endif
	return true;
}
DEFINE_COMMAND_PLUGIN(TestGetControlCopyPasta, "TestGetControlCopyPasta command", 0, 1, kParams_OneInt)
//### TestDisableKeyCopyPasta
bool Cmd_TestDisableKeyCopyPasta_Execute(COMMAND_ARGS)
{
	//Open
	*result = 0;
	UInt32	keycode = 0;
#if CC_Debug
	Debug_CC("TestDisableKeyCopyPasta`Open");
#endif
	//
	if (!ExtractArgs(paramInfo, arg1, opcodeOffsetPtr, thisObj, arg3, scriptObj, eventList, &keycode)) return true;
	if (keycode % 256 == 255 && keycode < 2048) keycode = 255 + (keycode + 1) / 256;
	if (IsKeycodeValid(keycode)) DI_data.DisallowStates[keycode] = 0x00;
	//Close
#if CC_Debug
	Debug_CC("TestDisableKeyCopyPasta`Close");
#endif
	return true;
}
DEFINE_COMMAND_PLUGIN(TestDisableKeyCopyPasta, "TestDisableKeyCopyPasta command", 0, 0, NULL)
//### GenerateEnum
bool Cmd_GenerateEnum_Execute(COMMAND_ARGS)
{
	//Open
#if CC_Debug
	Debug_CC("GenerateEnum`Open");
#endif
	//Extract Args
	TESForm* rTemp = NULL;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &rTemp)) {
#if CC_Debug
		Debug_CC("Cmd_DisableKey_Replacing_Execute`Failed arg extraction");
#endif
		return false;
	}
	//Report
#if CC_Debug
	Debug_CC("rTemp:" + TM_CommonCPP::Narrate(rTemp->refID));
#endif
	//Return
	*result = rTemp->refID;
	//Close
#if CC_Debug
	Debug_CC("GenerateEnum`Close");
#endif
	return true;
}
DEFINE_COMMAND_PLUGIN(GenerateEnum, "GenerateEnum command", 0, 1, kParams_OneObjectRef)
//### DisableKey_Replacing
bool Cmd_DisableKey_Replacing_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, UInt32 arg3, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
{
	//Open
#if CC_Debug
	Debug_CC("Cmd_DisableKey_Replacing_Execute`Open");
#endif
	UInt32	dxScancode = 0;
	UInt8	iModIndex = 0;
	*result = 0; //Do I need this?
	//---Register in Controls
	//-Get dxScancode
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &dxScancode)) {
#if CC_Debug
		Debug_CC("Cmd_DisableKey_Replacing_Execute`Failed arg extraction");
#endif
		return false;
	}
	//-Get iModIndex
	iModIndex = (UInt8)(scriptObj->refID >> 24);
#if CC_Debug
	Debug_CC("Cmd_DisableKey_Replacing_Execute`iModIndex:" + TM_CommonCPP::Narrate(iModIndex));
#endif
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
	//Close
#if CC_Debug
	Debug_CC("Controls:" + TM_CommonCPP::Narrate(Controls));
	Debug_CC("Cmd_DisableKey_Replacing_Execute`Close");
#endif
	return true;
}
DEFINE_COMMAND_PLUGIN(DisableKey_Replacing, "Disables a key and registers a disable", 0, 1, kParams_OneInt)
//### EnableKey_Replacing
bool Cmd_EnableKey_Replacing_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, UInt32 arg3, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
{
	//Open
#if CC_Debug
	Debug_CC("Cmd_EnableKey_Replacing_Execute`Open");
#endif
	UInt32	dxScancode = 0;
	UInt8	iModIndex = 0;
	*result = 0; //Do I need this?
	//---Register in Controls
	//-Get dxScancode
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &dxScancode)) {
#if CC_Debug
		Debug_CC("Cmd_EnableKey_Replacing_Execute`Failed arg extraction");
#endif
		return false;
	}
	//-Get iModIndex
	iModIndex = (UInt8)(scriptObj->refID >> 24);
	//---EnableKey
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
	//Execute original EnableKey
	if (bDoEnableKey) {
		EnableKey_OriginalExecute(PASS_COMMAND_ARGS);
	}
	//Close
#if CC_Debug
	Debug_CC(TM_CommonCPP::Narrate(Controls));
	Debug_CC("Cmd_EnableKey_Replacing_Execute`Close");
#endif
	return true;
}
DEFINE_COMMAND_PLUGIN(EnableKey_Replacing, "Unregisters disable of this mod. Enables key if there are no disables registered.", 0, 1, kParams_OneInt)
#pragma endregion
#pragma region LoadEvent
extern "C" {
bool OBSEPlugin_Query(const OBSEInterface * obse, PluginInfo * info)
{
	_MESSAGE("query");
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

	_MESSAGE("OBSE version too old (got %08X expected at least %08X)", obse->obseVersion, OBSE_VERSION_INTEGER);
	_MESSAGE("OBSE version too old (got %i expected at least %i)", obse->obseVersion, OBSE_VERSION_INTEGER);
	if (obse->obseVersion < OBSE_VERSION_INTEGER)
	{
		_MESSAGE("OBSE version too old (got %08X expected at least %08X)", obse->obseVersion, OBSE_VERSION_INTEGER);
		_MESSAGE("OBSE version too old (got %i expected at least %i)", obse->obseVersion, OBSE_VERSION_INTEGER);
		//return false;
	}

	return true;
}
bool OBSEPlugin_Load(const OBSEInterface * obse)
{
	_MESSAGE("load");
	obse->SetOpcodeBase(0x28B0);
	obse->RegisterCommand(&kCommandInfo_BasicRuntimeTests);
	obse->RegisterCommand(&kCommandInfo_TestGetControlDirectly);
	obse->RegisterCommand(&kCommandInfo_GenerateEnum);
	obse->RegisterCommand(&kCommandInfo_CommandTemplate);
	obse->RegisterCommand(&kCommandInfo_TestGetControlCopyPasta);
	obse->RegisterCommand(&kCommandInfo_TestDisableKeyCopyPasta);
	obse->RegisterCommand(&kCommandInfo_TestCeil);

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

	//Register Controls
	Controls.push_back(Control(17,Control::Forward));

	return true;
}
};
#pragma endregion
