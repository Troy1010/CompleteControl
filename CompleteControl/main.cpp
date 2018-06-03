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
#define CC_Debug 1
#include "obse/Script.h"

IDebugLog		gLog("CompleteControl.log"); //Log
Cmd_Execute DisableKey_OriginalExecute = NULL; //Execute of replaced DisableKey command
static std::vector<Control> Controls;
//-------HelperFunctions
void Debug_CC(std::string sString)
{
#if CC_Debug
	Console_Print(sString.c_str());
	_MESSAGE(sString.c_str());
#endif
}
//-------Commands
// Tester1
bool Cmd_Tester1_Execute(COMMAND_ARGS)
{
	//Open
	Debug_CC("Tester1`Open");
	*result = 0; //Do I need this?
	//
	//kCommandInfo_DisableKey_Original->execute(PASS_COMMAND_ARGS);
	//Close
	return true;
}
DEFINE_COMMAND_PLUGIN(Tester1, "Tester1 command", 0, 1, kParams_OneInt)
// DisableKey_Replacing
bool Cmd_DisableKey_Replacing_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, UInt32 arg3, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
{
	//Open
	Debug_CC("Cmd_DisableKey_Replacing_Execute`Open");
	UInt32	dxScancode = 0;
	UInt8	iModIndex = 0;
	*result = 0; //Do I need this?
	//---Register in Controls
	//-Get dxScancode
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &dxScancode)) {
		Debug_CC("Cmd_DisableKey_Replacing_Execute`Failed arg extraction");
		return true;
	}
	else
	{
		Debug_CC("Cmd_DisableKey_Replacing_Execute`Succeeded arg extraction");
	}
	Debug_CC("scriptObj->refID:"+ to_string_hex(scriptObj->refID));
	//scriptObj->refID;
	//-Get iModIndex
	//Debug_CC("Cmd_DisableKey_Replacing_Execute`GetModIndex`Open");
	//thisObj->baseForm;
	////g_commandTableIntfc->GetByName("GetSourceModIndex")->execute(PASS_COMMAND_ARGS);
	//TESForm* form = NULL;
	//form = thisObj;
	//if (form->IsCloned())
	//	*result = 0xFF;
	//else
	iModIndex = (UInt8)(scriptObj->refID >> 24);
	Debug_CC("iModIndex:" + to_string_hex(iModIndex));
	Debug_CC("Cmd_DisableKey_Replacing_Execute`GetModIndex`Close");
	//-Register iModIndex in vControl.cModIndices
	Debug_CC("Cmd_DisableKey_Replacing_Execute`Register`Open");
	for each (Control vControl in Controls)
	{
		if (vControl.dxScancode == dxScancode)
		{
			vControl.cModIndices.insert(iModIndex);
			break;
		}
	}
	Debug_CC("Cmd_DisableKey_Replacing_Execute`Register`Close");
	//---DisableKey
	//-Execute Original DisableKey
	Debug_CC("Cmd_DisableKey_Replacing_Execute`DisableKey`Open");
	DisableKey_OriginalExecute(PASS_COMMAND_ARGS);
	Debug_CC("Cmd_DisableKey_Replacing_Execute`DisableKey`Close");
	//Close
	Debug_CC("Cmd_DisableKey_Replacing_Execute`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(DisableKey_Replacing, "Disables a key and registers", 0, 1, kParams_OneInt)

//-------Load
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

	if (!obse->isEditor)
	{
		// get an OBSEScriptInterface to use for argument extraction
		g_scriptIntfc = (OBSEScriptInterface*)obse->QueryInterface(kInterface_Script);
		g_commandTableIntfc = (OBSECommandTableInterface*)obse->QueryInterface(kInterface_CommandTable);
		// replace DisableKey
		DisableKey_OriginalExecute = g_commandTableIntfc->GetByOpcode(0x1430)->execute; //DisableKey_opcode:00001430
		g_commandTableIntfc->Replace(0x1430, &kCommandInfo_DisableKey_Replacing);
	}

	return true;
}
};
