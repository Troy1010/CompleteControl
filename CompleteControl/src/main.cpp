#pragma region Includes
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
#include <set>
#include "Control.h"
#include <string>
#include "TM_CommonCPP/Misc.h"
#include "TM_CommonCPP/Narrate.h"
#include "TM_CommonCPP/StdStringFromFormatString.h"
#include "TM_CommonCPP_NarrateOverloads.h"
#include "obse/Script.h"
#include "obse/Hooks_DirectInput8Create.h"
#include <sstream>
#include "DebugCC.h"
#include "Settings.h"
#include "Globals.h"
#include "Misc.h"
#include "CopyPastedFromOBSE.h"
#include "Tests.h"
#include "CCCommands.h"
#pragma endregion

inline void NewGameOrLoadGame()
{
	bOblivionLoaded = true; //Because there is no OblivionLoaded event, I think this is the closest I can get. (without using mod helper)
}

#pragma region ModHandlers
//These 'handlers' are literally just commands called by the CompleteControlHelper.esp mod when its own event is fired.
bool Cmd_HandleOblivionLoaded_Execute(COMMAND_ARGS)
{
	DebugCC(5, "HandleOblivionLoaded`Open");
	//bOblivionLoaded = true;	//This would be a very good place to set this variable. However, I am trying to get rid of CC's reliance on a helper mod.
	//Controls = InitializeControls();
	DebugCC(5, "HandleOblivionLoaded`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(HandleOblivionLoaded, "HandleOblivionLoaded command", 0, 0, NULL)
//### HandleOnGameMode
bool Cmd_HandleOnGameMode_Execute(COMMAND_ARGS)
{
	DebugCC(7, std::string(__func__) + "`Open");
	bMenuMode = false;
	for (auto vControl : Controls)
	{
		vControl.SetOutcome();
	}
	DebugCC(7, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(HandleOnGameMode, "HandleOnGameMode command", 0, 0, NULL)
//### HandleOnMenuMode
bool Cmd_HandleOnMenuMode_Execute(COMMAND_ARGS)
{
	DebugCC(7, std::string(__func__) + "`Open");
	bMenuMode = true;
	for (auto vControl : Controls)
	{
		vControl.SetOutcome();
	}
	DebugCC(7, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(HandleOnMenuMode, "HandleOnMenuMode command", 0, 0, NULL)
#pragma endregion
#pragma region SerializationIntfcHandlers
void Handler_Save(void * reserved)
{
	DebugCC(5, "Handler_Save`Open");
	//-Write Control
	std::string sControls = StringizeControls(Controls);
	g_serialization->WriteRecord('CTRL', 0, sControls.c_str(), sControls.size());
	DebugCC(5, "Handler_Save`Close");
}

void Handler_Load(void * reserved)
{
	DebugCC(5, "Handler_Load`Open");
	DebugCC(6, "Controls:" + TMC::Narrate(Controls));
	NewGameOrLoadGame();
	//-Serialization
	UInt32	type, version, length;
	char* buf;
	while (g_serialization->GetNextRecordInfo(&type, &version, &length))
	{
		DebugCC(5, TMC::StdStringFromFormatString("record %08X (%.4s) %08X %08X", type, &type, version, length));
		switch (type)
		{
		case 'CTRL':
			buf = new char[length + 1]; //c strings require a null at the end.
			g_serialization->ReadRecordData(buf, length);
			buf[length] = 0;
			DebugCC(5, "buf:" + TMC::Narrate(buf));
			Controls = ControlsFromString(std::string(buf));
			delete buf;
			break;
		default:
			DebugCC(5, TMC::StdStringFromFormatString("Unknown chunk type %08X", type));
		}
	}
	// ResolveModIndices
	if (!Controls.empty())
	{
		for (auto& vControl : Controls)
		{
			vControl.ResolveModIndices();
		}
	}
	else //-For savegames written before CC install.
	{
		Controls = InitializeControls();
	}
	DebugCC(6, "Controls:" + TMC::Narrate(Controls));
	//---Refresh Disables
	SetOutcomeForAllControls(Controls);
	DebugCC(5, "Handler_Load`Close");
}

void Handler_Preload(void * reserved)
{
	DebugCC(5, "Handler_Preload`Open");
	DebugCC(5, "Handler_Preload`Close");
}

void Handler_NewGame(void * reserved)
{
	DebugCC(5, "Handler_NewGame`Open");
	NewGameOrLoadGame();
	Controls = InitializeControls();
	DebugCC(5, "Handler_NewGame`Close");
}
#pragma endregion
#pragma region MessageHandler
OBSEMessagingInterface* g_msg;

void MessageHandler(OBSEMessagingInterface::Message* msg)
{
	switch (msg->type)
	{
	case OBSEMessagingInterface::kMessage_ExitGame:
		DebugCC(5, "MessageHandler`received ExitGame message");
		break;
	case OBSEMessagingInterface::kMessage_ExitToMainMenu:
		DebugCC(5, "MessageHandler`received ExitToMainMenu message");
		break;
	case OBSEMessagingInterface::kMessage_PostLoad: //PostPluginsLoad
		DebugCC(5, "MessageHandler`received PostLoad message");
		break;
	case OBSEMessagingInterface::kMessage_LoadGame:
	case OBSEMessagingInterface::kMessage_SaveGame:
		DebugCC(5, "MessageHandler`received save/load message with file name:" + TMC::RSplitString(TMC::StdStringFromFormatString("%s", msg->data), "\\", 1).back());
		break;
	case OBSEMessagingInterface::kMessage_Precompile:
	{
		ScriptBuffer* buffer = (ScriptBuffer*)msg->data;
		DebugCC(5, TMC::StdStringFromFormatString("MessageHandler`received precompile message. Script Text:\n%s", buffer->scriptText));
		break;
	}
	case OBSEMessagingInterface::kMessage_PreLoadGame:
		DebugCC(5, "MessageHandler`received pre-loadgame message with file name:" + TMC::RSplitString(TMC::StdStringFromFormatString("%s", msg->data), "\\", 1).back());
		break;
	case OBSEMessagingInterface::kMessage_ExitGame_Console:
		DebugCC(5, "MessageHandler`received quit game from console message");
		break;
	case OBSEMessagingInterface::kMessage_PostLoadGame:
		DebugCC(5, "MessageHandler`received PostLoadGame message");
		break;
	case OBSEMessagingInterface::kMessage_PostPostLoad:
		DebugCC(5, "MessageHandler`received PostPostLoad message");
		break;
	case OBSEMessagingInterface::kMessage_RuntimeScriptError:
		DebugCC(5, "MessageHandler`received RuntimeScriptError message");
		break;
	default:
		DebugCC(5, "Plugin Example received unknown message. typeID:" + TMC::Narrate(msg->type));
		break;
	}
}
#pragma endregion
#pragma region LoadCCHandler
extern "C" {
bool OBSEPlugin_Query(const OBSEInterface * obse, PluginInfo * info)
{
	DebugCC(5, std::string(__func__) + "`Open");
	/*InterfaceManager* intfc = InterfaceManager::GetSingleton();
	DebugCC(5, "IsConsoleMode:"+ TMC::Narrate(intfc->IsGameMode()));*/
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

	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
bool OBSEPlugin_Load(const OBSEInterface * obse)
{
	DebugCC(5, std::string(__func__) + "`Open");
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
		g_serialization->SetPreloadCallback(g_pluginHandle, Handler_Preload);
	}
	obse->SetOpcodeBase(0x28B0);
	obse->RegisterCommand(&kCommandInfo_RegisterControl);
	obse->RegisterCommand(&kCommandInfo_IsDisabled);
	obse->RegisterCommand(&kCommandInfo_GetKey);
	obse->RegisterCommand(&kCommandInfo_UnreportedDisable);
	obse->RegisterCommand(&kCommandInfo_UnreportedEnable);
	obse->RegisterCommand(&kCommandInfo_OnControlDown);
	obse->RegisterCommand(&kCommandInfo_OnControlDown_Ref);

	obse->RegisterCommand(&kCommandInfo_HandleOblivionLoaded);
	obse->RegisterCommand(&kCommandInfo_HandleOnGameMode);
	obse->RegisterCommand(&kCommandInfo_HandleOnMenuMode);

	obse->RegisterCommand(&kCommandInfo_BasicRuntimeTests);
	obse->RegisterCommand(&kCommandInfo_TestGetControlDirectly);
	obse->RegisterCommand(&kCommandInfo_TestGetControlDirectly2);
	obse->RegisterCommand(&kCommandInfo_GenerateEnum);
	obse->RegisterCommand(&kCommandInfo_CommandTemplate);
	obse->RegisterCommand(&kCommandInfo_TestGetControlCopyPasta);
	obse->RegisterCommand(&kCommandInfo_TestDisableKeyCopyPasta);
	obse->RegisterCommand(&kCommandInfo_TestCeil);
	obse->RegisterCommand(&kCommandInfo_PrintControls);
	obse->RegisterCommand(&kCommandInfo_TestControlToString);
	obse->RegisterCommand(&kCommandInfo_TestControlsFromString);

	if (!obse->isEditor)
	{
		// get an OBSEScriptInterface to use for argument extraction
		g_scriptIntfc = (OBSEScriptInterface*)obse->QueryInterface(kInterface_Script);
		g_commandTableIntfc = (OBSECommandTableInterface*)obse->QueryInterface(kInterface_CommandTable);
		// replace DisableKey
		DisableKey_OriginalExecute = g_commandTableIntfc->GetByOpcode(0x1430)->execute; //DisableKey_opcode:0x1430
		g_commandTableIntfc->Replace(0x1430, &kCommandInfo_DisableKey_Replacing);
		// replace EnableKey
		EnableKey_OriginalExecute = g_commandTableIntfc->GetByOpcode(0x1431)->execute; //EnableKey_opcode:0x1431
		g_commandTableIntfc->Replace(0x1431, &kCommandInfo_EnableKey_Replacing);
		// replace DisableControl
		g_commandTableIntfc->Replace(0x15A7, &kCommandInfo_DisableControl_Replacing); //DisableControl_opcode:0x15A7
		// replace EnableControl
		g_commandTableIntfc->Replace(0x15A8, &kCommandInfo_EnableControl_Replacing); //EnableControl_opcode:0x15A8
		// Get Some Commands
		GetControl_CmdInfo = g_commandTableIntfc->GetByOpcode(0x146A); //GetControl_opcode:0x146A
		ResolveModIndex_CmdInfo = g_commandTableIntfc->GetByOpcode(0x19A8); // ResolveModIndex_opcode:0x19A8
		IsKeyPressed3_CmdInfo = g_commandTableIntfc->GetByName("IsKeyPressed3");
	}

	// register to receive messages from OBSE
	OBSEMessagingInterface* msgIntfc = (OBSEMessagingInterface*)obse->QueryInterface(kInterface_Messaging);
	msgIntfc->RegisterListener(g_pluginHandle, "OBSE", MessageHandler);
	g_msg = msgIntfc;

	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
};
#pragma endregion
