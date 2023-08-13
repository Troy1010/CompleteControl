#include "Globals.h"
#include "obse/PluginAPI.h"
#include "obse/CommandTable.h"
#include "obse_common/SafeWrite.cpp"
#if OBLIVION
#include "obse/GameAPI.h"
#else
#include "obse_editor/EditorAPI.h"
#endif
#include "obse/ParamInfos.h"
#include <vector>
#include "Control.h"
// #include "obse/Hooks_DirectInput8Create.h"


using namespace std;
IDebugLog		gLog("CompleteControl.log");

const CommandInfo * kCommandInfo_DisableKey = NULL;
const CommandInfo * kCommandInfo_EnableKey = NULL;
CommandInfo * kCommandInfo_DisableKey2_Pointer = NULL;

//// called from 004F90A5
//bool Cmd_Default_Parse2(UInt32 numParams, ParamInfo* paramInfo, ScriptLineBuffer* lineBuf, ScriptBuffer* scriptBuf)
//{
//#ifdef _DEBUG
//#if 0
//	_MESSAGE("Cmd_Default_Parse: %08X %08X %08X %08X",
//		arg0, arg1, arg2, arg3);
//#endif
//#endif
//
//#ifdef OBLIVION
//
//#if OBLIVION_VERSION == OBLIVION_VERSION_1_1
//	static const Cmd_Parse g_defaultParseCommand = (Cmd_Parse)0x004F38C0;
//#elif OBLIVION_VERSION == OBLIVION_VERSION_1_2
//	static const Cmd_Parse g_defaultParseCommand = (Cmd_Parse)0x004FDF80;
//#elif OBLIVION_VERSION == OBLIVION_VERSION_1_2_416
//	static const Cmd_Parse g_defaultParseCommand = (Cmd_Parse)0x004FDE30;
//#else
//#error unsupported version of oblivion
//#endif
//
//#else
//
//#if CS_VERSION == CS_VERSION_1_0
//	static const Cmd_Parse g_defaultParseCommand = (Cmd_Parse)0x004F69C0;
//#elif CS_VERSION == CS_VERSION_1_2
//	static const Cmd_Parse g_defaultParseCommand = (Cmd_Parse)0x00500FF0;
//#else
//#error unsupported cs version
//#endif
//
//#endif
//
//	// arg0 = idx?
//	// arg1 = ParamInfo *
//	// arg2 = ptr to line to parse, skip UInt32 header first
//	// arg3 = ptr to script info? first UInt32 is ptr to script data
//
//	return g_defaultParseCommand(numParams, paramInfo, lineBuf, scriptBuf);
//}


// DisableKey2
bool Cmd_DisableKey2_Execute(COMMAND_ARGS)
{
	//Console_Print("Cmd_DisableKey2_Execute`Open");
	*result = 0; //Do I need this?

//	kCommandInfo_DisableKey = g_commandTableIntfc->GetByOpcode(0x1430); //opcode:00001430

//	Console_Print(strcat3("kCommandInfo_DisableKey->shortName: ", kCommandInfo_DisableKey->shortName));
//	kCommandInfo_DisableKey->execute(PASS_COMMAND_ARGS);
	Console_Print("ReplacementCommand`Open");


	// Report Success
	return true;
}
//DEFINE_COMMAND_PLUGIN(DisableKey2, "Disables a key and registers this action", 0, 1, kParams_OneInt)
CommandInfo kCommandInfo_DisableKey2 =
{
	"DisableKey2",
	"dk2",
	0,
	"Prevents a player from using a key2",
	0,
	1,
	kParams_OneInt,
	HANDLER(Cmd_DisableKey2_Execute),
	NULL,//Cmd_Default_Parse2,
	NULL,
	0
};


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
	obse->RegisterCommand(&kCommandInfo_DisableKey2);
	obse->RegisterCommand(&kCommandInfo_EnableKey2);
	//obse->RegisterCommand(&kCommandInfo_TestExtractFormatString);
	if(!obse->isEditor)
	{
		// Get an OBSEScriptInterface to use for argument extraction
		g_scriptIntfc = (OBSEScriptInterface*)obse->QueryInterface(kInterface_Script);
		g_commandTableIntfc = (OBSECommandTableInterface*)obse->QueryInterface(kInterface_CommandTable);
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
