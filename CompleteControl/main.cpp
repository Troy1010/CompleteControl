#include "obse/PluginAPI.h"
#include "obse/CommandTable.h"
#include "obse_common/SafeWrite.cpp"
#if OBLIVION
#include "obse/GameAPI.h"
OBSEScriptInterface * g_scriptIntfc = NULL; // assigned in OBSEPlugin_Load
#define ExtractArgsEx(...) g_scriptIntfc->ExtractArgsEx(__VA_ARGS__)
#define ExtractFormatStringArgs(...) g_scriptIntfc->ExtractFormatStringArgs(__VA_ARGS__)
#else
#include "obse_editor/EditorAPI.h"
#endif
#include "obse/ParamInfos.h"
#include <vector>
#include "Control.h"
//#include "TM_CommonCPP/Misc.h"


IDebugLog		gLog("CompleteControl.log"); //Log
OBSECommandTableInterface	* g_commandTableIntfc = NULL; //For command argument extraction
Cmd_Execute DisableKey_OriginalExecute = NULL; //Execute of replaced DisableKey command
static std::vector<Control> Controls;

//-------Commands
// Tester1
bool Cmd_Tester1_Execute(COMMAND_ARGS)
{
	//Open
	Console_Print("Tester1`Open");
	*result = 0; //Do I need this?
	//
	//kCommandInfo_DisableKey_Original->execute(PASS_COMMAND_ARGS);
	//Close
	return true;
}
DEFINE_COMMAND_PLUGIN(Tester1, "Tester1 command", 0, 1, kParams_OneInt)
// DisableKey_Replacing
bool Cmd_DisableKey_Replacing_Execute(COMMAND_ARGS)
{
	//Open
	Console_Print("Cmd_DisableKey_Replacing_Execute`Open");
	*result = 0; //Do I need this?
	//Execute Original DisableKey
	DisableKey_OriginalExecute(PASS_COMMAND_ARGS);
	//Register in Controls

	//Close
	Console_Print("Cmd_DisableKey_Replacing_Execute`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(DisableKey_Replacing, "Disables a key and registers this action", 0, 1, kParams_OneInt)

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
