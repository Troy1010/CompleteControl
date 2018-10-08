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

IDebugLog		gLog("CompleteControl.log"); //Log
Cmd_Execute DisableKey_OriginalExecute = NULL; //Execute of replaced DisableKey command
Cmd_Execute EnableKey_OriginalExecute = NULL; //Execute of replaced DisableKey command
static std::vector<Control> Controls;
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
// Tester1
bool Cmd_Tester1_Execute(COMMAND_ARGS)
{
	//Open
#if CC_Debug
	Debug_CC("Tester1`Open");
#endif
	*result = 0; //Do I need this?
	int iInt = 5;
#if CC_Debug
	Debug_CC("5:" + TM_CommonCPP::Narrate(iInt));
#endif
	UInt8 vUInt8 = 3;
#if CC_Debug
	Debug_CC("3:" + TM_CommonCPP::Narrate(vUInt8));
#endif
	std::set<UInt8> cSet;
	cSet.insert(65);
	cSet.insert(64);
	cSet.insert(63);
#if CC_Debug
	Debug_CC("Set:" + TM_CommonCPP::Narrate(cSet));
	Debug_CC("ActualControls:" + TM_CommonCPP::Narrate(Controls));
#endif
	static std::vector<Control> Controls_Fake;
	Controls_Fake.push_back(15);
	for (Control &vControl : Controls_Fake)
	{
		vControl.cModIndices.insert(222);
	}
	//Close
#if CC_Debug
	Debug_CC("FakeControls:" + TM_CommonCPP::Narrate(Controls_Fake));
	Debug_CC("Tester1`Close");
#endif
	return true;
}
DEFINE_COMMAND_PLUGIN(Tester1, "Tester1 command", 0, 1, NULL)
// Tester2
bool Cmd_Tester2_Execute(COMMAND_ARGS)
{
	//Open
#if CC_Debug
	Debug_CC("Tester2`Open");
#endif
	//Extras
	*result = 0; //Do I need this?
	//
	for (Control &vControl : Controls)
	{
		vControl.cModIndices.insert(321);
		break;
	}
	//Close
#if CC_Debug
	Debug_CC("Tester2`Close");
#endif
	return true;
}
DEFINE_COMMAND_PLUGIN(Tester2, "Tester2 command", 0, 1, NULL)
// DisableKey_Replacing
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
			vControl.cModIndices.insert(iModIndex);
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
// EnableKey_Replacing
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
			vControl.cModIndices.erase(iModIndex);
			if (!(vControl.cModIndices.empty()))
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
	obse->RegisterCommand(&kCommandInfo_Tester1);
	obse->RegisterCommand(&kCommandInfo_Tester2);

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
	}

	//Register Controls
	Controls.push_back(Control(17));

	return true;
}
};
#pragma endregion
