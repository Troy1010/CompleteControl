#include "obse/PluginAPI.h"
#include "obse/CommandTable.h"
#include "obse_common/SafeWrite.cpp"
#if OBLIVION
#include "obse/GameAPI.h"
OBSEScriptInterface * g_scriptInterface = NULL; // assigned in OBSEPlugin_Load
#define ExtractArgsEx(...) g_scriptInterface->ExtractArgsEx(__VA_ARGS__)
#define ExtractFormatStringArgs(...) g_scriptInterface->ExtractFormatStringArgs(__VA_ARGS__)
#else
#include "obse_editor/EditorAPI.h"
#endif
#include "obse/ParamInfos.h"
#include <vector>
#include "Control.h"
#include "obse/Hooks_DirectInput8Create.h"
#include "TM_CommonCPP/Misc.h"


using namespace std;
IDebugLog		gLog("CompleteControl.log");

OBSECommandTableInterface	* g_commandTableIntfc = NULL;
const CommandInfo * kCommandInfo_DisableKey = NULL;
const CommandInfo * kCommandInfo_EnableKey = NULL;
CommandInfo * kCommandInfo_DisableKey2_Pointer = NULL;

static vector<Control> Controls;

// Tester1
bool Cmd_Tester1_Execute(COMMAND_ARGS)
{
	Console_Print("Tester1`Open");
	*result = 0; //Do I need this?
	Console_Print("ReplacementCommand`Open");
	g_commandTableIntfc->Replace(0x1430, kCommandInfo_DisableKey2_Pointer);

	// Report Success
	return true;
}
DEFINE_COMMAND_PLUGIN(Tester1, "Tester1 command", 0, 0, NULL)



// DisableKey2
bool Cmd_DisableKey2_Execute(COMMAND_ARGS)
{
	Console_Print("Cmd_DisableKey2_Execute`Open");
	*result = 0; //Do I need this?



	// Report Success
	Console_Print("Cmd_DisableKey2_Execute`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(DisableKey2, "Disables a key and registers this action", 0, 1, kParams_OneInt)


// EnableKey2
bool Cmd_EnableKey2_Execute(COMMAND_ARGS)
{
	*result = 0; //Do I need this?

	kCommandInfo_EnableKey->execute(PASS_COMMAND_ARGS);

	// Report Success
	return true;
}
DEFINE_COMMAND_PLUGIN(EnableKey2, "Enables a key if 0 mods have it disabled", 0, 1, kParams_OneInt)


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
//	obse->RegisterCommand(&kCommandInfo_DisableKey2);
	obse->RegisterCommand(&kCommandInfo_EnableKey2);
	//obse->RegisterCommand(&kCommandInfo_TestExtractFormatString);
	if(!obse->isEditor)
	{
		// get an OBSEScriptInterface to use for argument extraction
		g_scriptInterface = (OBSEScriptInterface*)obse->QueryInterface(kInterface_Script);
	}

	g_commandTableIntfc = (OBSECommandTableInterface*)obse->QueryInterface(kInterface_CommandTable);
//	kCommandInfo_DisableKey = g_commandTableIntfc->GetByName("DisableKey"); //opcode:00001430
	kCommandInfo_EnableKey = g_commandTableIntfc->GetByName("EnableKey"); //opcode:00001431
	kCommandInfo_DisableKey = g_commandTableIntfc->GetByOpcode(0x1430); //opcode:00001430
//	UInt32 opcode_1 = 0x1430;
	UInt32 opcode_1 = kCommandInfo_DisableKey->opcode;
	_MESSAGE("opcode_1: %08X", opcode_1);
	_MESSAGE("kCommandInfo_DisableKey->longName: %s", kCommandInfo_DisableKey->longName);


	/*kCommandInfo_DisableKey2.opcode = 0x00001434;
	_MESSAGE("kCommandInfo_DisableKey2.opcode: %08X", kCommandInfo_DisableKey2.opcode);
	kCommandInfo_DisableKey2_Pointer = &kCommandInfo_DisableKey2;
	_MESSAGE("kCommandInfo_DisableKey2_Pointer->opcode: %08X", kCommandInfo_DisableKey2_Pointer->opcode);
	kCommandInfo_DisableKey2_Pointer->opcode = 0x00001430;
	_MESSAGE("kCommandInfo_DisableKey2_Pointer->opcode: %08X", kCommandInfo_DisableKey2_Pointer->opcode);*/

	//kCommandInfo_DisableKey->parse
	//kCommandInfo_DisableKey2_Pointer
	kCommandInfo_DisableKey2_Pointer = &kCommandInfo_DisableKey2;
	_MESSAGE("a");
	kCommandInfo_DisableKey2_Pointer->eval = kCommandInfo_DisableKey->eval;
	_MESSAGE("b");
//	kCommandInfo_DisableKey2_Pointer->execute = kCommandInfo_DisableKey->execute;
	_MESSAGE("c");
	kCommandInfo_DisableKey2_Pointer->flags = kCommandInfo_DisableKey->flags;
	_MESSAGE("d");
	kCommandInfo_DisableKey2_Pointer->helpText = kCommandInfo_DisableKey->helpText;
	_MESSAGE("e");
	kCommandInfo_DisableKey2_Pointer->longName = kCommandInfo_DisableKey->longName;
	_MESSAGE("f");
	kCommandInfo_DisableKey2_Pointer->needsParent = kCommandInfo_DisableKey->needsParent;
	_MESSAGE("g");
	kCommandInfo_DisableKey2_Pointer->numParams = kCommandInfo_DisableKey->numParams;
	_MESSAGE("h");
	kCommandInfo_DisableKey2_Pointer->opcode = kCommandInfo_DisableKey->opcode;
	_MESSAGE("i");
	kCommandInfo_DisableKey2_Pointer->params = kCommandInfo_DisableKey->params;
	_MESSAGE("j");
	kCommandInfo_DisableKey2_Pointer->shortName = kCommandInfo_DisableKey->shortName;
	_MESSAGE("k");
	//kCommandInfo_DisableKey2_Pointer = new CommandInfo(kCommandInfo_DisableKey);

	_MESSAGE("Replace`Open");
	try
	{
		if (!obse->isEditor)
		{
			g_commandTableIntfc->Replace(opcode_1, kCommandInfo_DisableKey2_Pointer);
		}
	}
	catch (...)
	{
		_ERROR("Replace ERROR");
		throw;
	}
	/*catch (exception& e)
	{
		_MESSAGE("ERROR:%s", e.what());
		throw;
	}*/
//	g_commandTableIntfc->Replace(opcode_1, kCommandInfo_DisableKey2_Pointer);
//	g_commandTableIntfc->Replace(kCommandInfo_DisableKey->opcode, &kCommandInfo_DisableKey2);
//	g_commandTableIntfc->Replace(0x1430, &kCommandInfo_DisableKey2); //This one is my fav

//	CommandInfo* kCommandInfo_DisableKey_NotConst = const_cast<CommandInfo*>(kCommandInfo_DisableKey);
//	g_commandTableIntfc->Replace(opcode_1, kCommandInfo_DisableKey_NotConst);
	_MESSAGE("Replace`Close");

	return true;
}
};
