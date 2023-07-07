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
#include "TM_CommonCPP/ToLogStr.h"
#include "TM_CommonCPP/StdStringFromFormatString.h"
#include "TM_CommonCPP_NarrateOverloads.h"
#include "obse/Script.h"
// #include "obse/Hooks_DirectInput8Create.h"
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
	Logd("HandleOblivionLoaded`Open");
	//bOblivionLoaded = true;	//This would be a nice place to set this variable. However, I am trying to get rid of CC's reliance on a helper mod.
	//Controls = InitializeControls();
	Logd("HandleOblivionLoaded`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(HandleOblivionLoaded, "HandleOblivionLoaded command", 0, 0, nullptr)
//### HandleOnGameMode
bool Cmd_HandleOnGameMode_Execute(COMMAND_ARGS)
{
	Logv(std::string(__func__) + "`Open");
	Controls.EnableAll();
	Controls.SetOutcomes();
	Logv(std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(HandleOnGameMode, "HandleOnGameMode command", 0, 0, nullptr)
//### HandleOnMenuMode
bool Cmd_HandleOnMenuMode_Execute(COMMAND_ARGS)
{
	Logv(std::string(__func__) + "`Open");
	Controls.EnableAll();
	Controls.SetOutcomes();
	Logv(std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(HandleOnMenuMode, "HandleOnMenuMode command", 0, 0, nullptr)
#pragma endregion
#pragma region SerializationIntfcHandlers
void Handler_Save(void * reserved)
{
	Logd(std::string(__func__) + "`Open");
	//-Write Control
	std::string sControls = Controls.Stringize();
	g_serialization->WriteRecord('CTRL', 0, sControls.c_str(), sControls.size());
	Logd(std::string(__func__) + "`Close");
}

void Handler_Load(void * reserved)
{
	Logd(std::string(__func__) + "`Open");
	Logv("Controls:" + TMC::ToLogStr(Controls));
	NewGameOrLoadGame();
	//-Serialization
	UInt32	type, version, length;
	char* buf;
	while (g_serialization->GetNextRecordInfo(&type, &version, &length))
	{
		Logv(TMC::StdStringFromFormatString("record %08X (%.4s) %08X %08X", type, &type, version, length)); //Record type is.. reversed? why?
		switch (type)
		{
		case 'CTRL':
			buf = new char[length + 1]; //c strings require a null at the end.
			g_serialization->ReadRecordData(buf, length);
			buf[length] = 0;
			Logv("buf:" + TMC::ToLogStr(buf));
			Controls = ControlCollection(std::string(buf));
			delete buf;
			break;
		default:
			Logd(TMC::StdStringFromFormatString("Unknown chunk type %08X", type));
		}
	}
	//-ResolveModIndices
	if (!Controls.Items.empty())
	{
		for (auto& vControl : Controls.Items)
		{
			vControl.second.ResolveModIndices();
		}
	}
	else //-For savegames written before CC install.
	{
		Controls.RegisterVanillaControls();
	}
	Logv("Controls:" + TMC::ToLogStr(Controls));
	//---Refresh Disables
	Controls.SetOutcomes();
	Logd(std::string(__func__) + "`Close");
}

void Handler_Preload(void * reserved)
{
	Logd(std::string(__func__) + "`Open");
	Logd(std::string(__func__) + "`Close");
}

void Handler_NewGame(void * reserved)
{
	bOblivionLoaded = true; // for sanity, to see the following debug message.
	Logd(std::string(__func__) + "`Open");
	NewGameOrLoadGame();
	Controls.RegisterVanillaControls();
	Logd(std::string(__func__) + "`Close");
}
#pragma endregion
#pragma region MessageHandler
OBSEMessagingInterface* g_msg;

void MessageHandler(OBSEMessagingInterface::Message* msg)
{
	switch (msg->type)
	{
	case OBSEMessagingInterface::kMessage_ExitGame:
		Logd("MessageHandler`received ExitGame message");
		break;
	case OBSEMessagingInterface::kMessage_ExitToMainMenu:
		Logd("MessageHandler`received ExitToMainMenu message");
		break;
	case OBSEMessagingInterface::kMessage_PostLoad: //PostPluginsLoad
		Logd("MessageHandler`received PostLoad message");
		break;
	case OBSEMessagingInterface::kMessage_LoadGame:
	case OBSEMessagingInterface::kMessage_SaveGame:
		Logd("MessageHandler`received save/load message with file name:" + TMC::Str::RSplit(TMC::StdStringFromFormatString("%s", msg->data), "\\", 1).back());
		break;
	case OBSEMessagingInterface::kMessage_Precompile:
	{
		ScriptBuffer* buffer = (ScriptBuffer*)msg->data;
		Logd(TMC::StdStringFromFormatString("MessageHandler`received precompile message. Script Text:\n%s", buffer->scriptText));
		break;
	}
	case OBSEMessagingInterface::kMessage_PreLoadGame:
		Logd("MessageHandler`received pre-loadgame message with file name:" + TMC::Str::RSplit(TMC::StdStringFromFormatString("%s", msg->data), "\\", 1).back());
		break;
	case OBSEMessagingInterface::kMessage_ExitGame_Console:
		Logd("MessageHandler`received quit game from console message");
		break;
	case OBSEMessagingInterface::kMessage_PostLoadGame:
		Logd("MessageHandler`received PostLoadGame message");
		break;
	case OBSEMessagingInterface::kMessage_PostPostLoad:
		Logd("MessageHandler`received PostPostLoad message");
		break;
	case OBSEMessagingInterface::kMessage_RuntimeScriptError:
		Logd("MessageHandler`received RuntimeScriptError message");
		break;
	default:
		Logd("Plugin Example received unknown message. typeID:" + TMC::ToLogStr(msg->type));
		break;
	}
}
#pragma endregion
#pragma region LoadCCHandler
extern "C" {
bool OBSEPlugin_Query(const OBSEInterface * obse, PluginInfo * info)
{
	Logd(std::string(__func__) + "`Open");
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "CompleteControl";
	info->version = 1;
	if(!obse->isEditor)
	{
#if OBLIVION
		if (obse->oblivionVersion != OBLIVION_VERSION) { _ERROR("incorrect Oblivion version (got %08X need %08X)", obse->oblivionVersion, OBLIVION_VERSION); return false; }
#endif	
		if (!(g_arrayIntfc = (OBSEArrayVarInterface*)obse->QueryInterface(kInterface_ArrayVar))) { _ERROR("Array interface not found"); return false; }
	}
	if (obse->obseVersion < OBSE_VERSION_INTEGER) { _MESSAGE("OBSE version too old (got %08X expected at least %08X)", obse->obseVersion, OBSE_VERSION_INTEGER); return false; }
	Logd(std::string(__func__) + "`Close");
	return true;
}
bool OBSEPlugin_Load(const OBSEInterface * obse)
{
	Logd(std::string(__func__) + "`Open");
	g_pluginHandle = obse->GetPluginHandle();
	if (!obse->isEditor)
	{
		if (!(g_serialization = (OBSESerializationInterface *)obse->QueryInterface(kInterface_Serialization))) { _ERROR("serialization interface not found"); return false; }
		if (g_serialization->version < OBSESerializationInterface::kVersion) { _ERROR("incorrect serialization version found (got %08X need %08X)", g_serialization->version, OBSESerializationInterface::kVersion); return false; }
		g_serialization->SetSaveCallback(g_pluginHandle, Handler_Save);
		g_serialization->SetLoadCallback(g_pluginHandle, Handler_Load);
		g_serialization->SetNewGameCallback(g_pluginHandle, Handler_NewGame);
		g_serialization->SetPreloadCallback(g_pluginHandle, Handler_Preload);
	}
	obse->SetOpcodeBase(0x28B0);
	obse->RegisterCommand(&kCommandInfo_DisableControl_ByRef);
	obse->RegisterCommand(&kCommandInfo_DisableControls);
	obse->RegisterCommand(&kCommandInfo_EnableControl_ByRef);
	obse->RegisterCommand(&kCommandInfo_EnableControls);
	obse->RegisterCommand(&kCommandInfo_RegisterControl);
	obse->RegisterCommand(&kCommandInfo_IsDisabled);
	obse->RegisterCommand(&kCommandInfo_IsDisabled_ByRef);
	obse->RegisterCommand(&kCommandInfo_GetKey);
	obse->RegisterCommand(&kCommandInfo_GetKey_ByRef);
	obse->RegisterCommand(&kCommandInfo_UnreportedDisable);
	obse->RegisterCommand(&kCommandInfo_UnreportedDisable_ByRef);
	obse->RegisterCommand(&kCommandInfo_UnreportedEnable);
	obse->RegisterCommand(&kCommandInfo_UnreportedEnable_ByRef);
	obse->RegisterCommand(&kCommandInfo_IsEngaged);
	obse->RegisterCommand(&kCommandInfo_IsEngaged_ByRef);
	obse->RegisterCommand(&kCommandInfo_IsPressed);
	obse->RegisterCommand(&kCommandInfo_IsPressed_ByRef);
	obse->RegisterCommand(&kCommandInfo_OnControlDown2);
	obse->RegisterCommand(&kCommandInfo_OnControlDown2_ByRef);

	obse->RegisterCommand(&kCommandInfo_HandleOblivionLoaded);
	obse->RegisterCommand(&kCommandInfo_HandleOnGameMode);
	obse->RegisterCommand(&kCommandInfo_HandleOnMenuMode);

	// TODO: These would be better suited as replacements. However, the g_commandTableIntfc->Replace() command was not working.. so as a duct-tape solution, client mods must use DisableKey2, EnableKey2, etc.
	obse->RegisterCommand(&kCommandInfo_DisableKey2);
	obse->RegisterCommand(&kCommandInfo_EnableKey2);
	obse->RegisterCommand(&kCommandInfo_DisableControl2);
	obse->RegisterCommand(&kCommandInfo_EnableControl2);

	if (!obse->isEditor)
	{
		// Get an OBSEScriptInterface to use for argument extraction
		g_scriptIntfc = (OBSEScriptInterface*)obse->QueryInterface(kInterface_Script);
		g_commandTableIntfc = (OBSECommandTableInterface*)obse->QueryInterface(kInterface_CommandTable);
		// Get original execute functions of DisableKey, EnableKey
		DisableKey_OriginalExecute = g_commandTableIntfc->GetByOpcode(0x1430)->execute; //DisableKey_opcode:0x1430
		EnableKey_OriginalExecute = g_commandTableIntfc->GetByOpcode(0x1431)->execute; //EnableKey_opcode:0x1431
		// // Replace DisableKey, EnableKey
		// g_commandTableIntfc->Replace(0x1430, &kCommandInfo_DisableKey_Replacing);
		// g_commandTableIntfc->Replace(0x1431, &kCommandInfo_EnableKey_Replacing);
		// // Replace DisableControl, EnableControl
		// g_commandTableIntfc->Replace(0x15A7, &kCommandInfo_DisableControl_Replacing); //DisableControl_opcode:0x15A7
		// g_commandTableIntfc->Replace(0x15A8, &kCommandInfo_EnableControl_Replacing); //EnableControl_opcode:0x15A8
		// Get some commands we might use with ExecuteCommand
		GetControl_CmdInfo = g_commandTableIntfc->GetByOpcode(0x146A); //GetControl_opcode:0x146A
		GetAltControl2_CmdInfo = g_commandTableIntfc->GetByName("GetAltControl2");
		ResolveModIndex_CmdInfo = g_commandTableIntfc->GetByOpcode(0x19A8); // ResolveModIndex_opcode:0x19A8
		IsKeyPressed3_CmdInfo = g_commandTableIntfc->GetByName("IsKeyPressed3");
	}

	//-Register to receive messages from OBSE
	OBSEMessagingInterface* msgIntfc = (OBSEMessagingInterface*)obse->QueryInterface(kInterface_Messaging);
	msgIntfc->RegisterListener(g_pluginHandle, "OBSE", MessageHandler);
	g_msg = msgIntfc;

	Logd(std::string(__func__) + "`Close");
	return true;
}
};
#pragma endregion
