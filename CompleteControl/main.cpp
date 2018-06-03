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
#include "TM_CommonCPP/Misc.h"


using namespace std;
IDebugLog		gLog("CompleteControl.log");

OBSECommandTableInterface	* g_commandTableIntfc = NULL;
const CommandInfo * kCommandInfo_DisableKey_Original = NULL;
const CommandInfo * kCommandInfo_EnableKey_Original = NULL;
CommandInfo * kCommandInfo_DisableKey2_Pointer = NULL;
CommandInfo kCommandInfo_DisableKey_Original2;

static vector<Control> Controls;

void CopyCmdInfo(CommandInfo Receiver, const CommandInfo * Giver)
{
	Receiver.eval = Giver->eval;
	Receiver.execute = Giver->execute;
	Receiver.flags = Giver->flags;
	Receiver.helpText = Giver->helpText;
	Receiver.longName = Giver->longName;
	Receiver.needsParent = Giver->needsParent;
	Receiver.numParams = Giver->numParams;
	Receiver.opcode = Giver->opcode;
	Receiver.params = Giver->params;
	Receiver.parse = Giver->parse;
	Receiver.shortName = Giver->shortName;
}

// Tester1
bool Cmd_Tester1_Execute(COMMAND_ARGS)
{
	Console_Print("Tester1`Open");
	*result = 0; //Do I need this?
	kCommandInfo_DisableKey_Original->execute(PASS_COMMAND_ARGS);
//	Console_Print("ReplacementCommand`Open");
//	g_commandTableIntfc->Replace(0x1430, kCommandInfo_DisableKey2_Pointer);

	// Report Success
	return true;
}
DEFINE_COMMAND_PLUGIN(Tester1, "Tester1 command", 0, 1, kParams_OneInt)


// DisableKey3 Access Old
bool Cmd_DisableKey3_Execute(COMMAND_ARGS)
{
	Console_Print("Cmd_DisableKey3_Execute`Open");
	*result = 0; //Do I need this?

				 //Execute DisableKey_Original
				 //kCommandInfo_DisableKey_Original->execute(PASS_COMMAND_ARGS);
	kCommandInfo_DisableKey_Original2.execute(PASS_COMMAND_ARGS);


	// Report Success
	Console_Print("Cmd_DisableKey3_Execute`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(DisableKey3, "Disables a key and registers this action", 0, 1, kParams_OneInt)

// DisableKey2 Replacing
bool Cmd_DisableKey2_Execute(COMMAND_ARGS)
{
	Console_Print("Cmd_DisableKey2_Execute`Open");
	*result = 0; //Do I need this?

	//Execute DisableKey_Original
	//kCommandInfo_DisableKey_Original->execute(PASS_COMMAND_ARGS);
	kCommandInfo_DisableKey3.execute(PASS_COMMAND_ARGS);


	// Report Success
	Console_Print("Cmd_DisableKey2_Execute`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(DisableKey2, "Disables a key and registers this action", 0, 1, kParams_OneInt)


// EnableKey2
bool Cmd_EnableKey2_Execute(COMMAND_ARGS)
{
	*result = 0; //Do I need this?

	kCommandInfo_EnableKey_Original->execute(PASS_COMMAND_ARGS);

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

	if(!obse->isEditor)
	{
		// get an OBSEScriptInterface to use for argument extraction
		g_scriptInterface = (OBSEScriptInterface*)obse->QueryInterface(kInterface_Script);
	}

	g_commandTableIntfc = (OBSECommandTableInterface*)obse->QueryInterface(kInterface_CommandTable);
	kCommandInfo_DisableKey_Original = g_commandTableIntfc->GetByName("DisableKey"); //opcode:00001430

	kCommandInfo_DisableKey3.execute = kCommandInfo_DisableKey_Original->execute;
	obse->RegisterCommand(&kCommandInfo_DisableKey3);



//	CopyCmdInfo(kCommandInfo_DisableKey_Original2, kCommandInfo_DisableKey_Original);
	//kCommandInfo_DisableKey_Original2.opcode = 0x28B6;
//	obse->RegisterCommand(&kCommandInfo_DisableKey_Original2);


	_MESSAGE("Replace`Open");
	try
	{
		if (!obse->isEditor)
		{
			g_commandTableIntfc->Replace(0x1430, &kCommandInfo_DisableKey2);
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
