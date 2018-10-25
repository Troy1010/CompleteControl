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
#include "EventHandlers.h"
#include "CopyPastedFromOBSE.h"
#include "Tests.h"
#pragma endregion
#pragma region CompleteControlAPI
//### CommandTemplate
bool Cmd_CommandTemplate_Execute(COMMAND_ARGS)
{
	DebugCC(5,"CommandTemplate`Open");
	DebugCC(5,"CommandTemplate`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(CommandTemplate, "CommandTemplate command", 0, 0, NULL)
//### HandleOblivionLoaded
bool Cmd_HandleOblivionLoaded_Execute(COMMAND_ARGS)
{
	DebugCC(5,"HandleOblivionLoaded`Open");
	bOblivionLoaded = true;
	//Controls = InitializeControls();
	DebugCC(5,"HandleOblivionLoaded`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(HandleOblivionLoaded, "HandleOblivionLoaded command", 0, 0, NULL)
//### DisableKey_Replacing
bool Cmd_DisableKey_Replacing_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, UInt32 arg3, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
{
	DebugCC(5,"Cmd_DisableKey_Replacing_Execute`Open");
	UInt32	dxScancode = 0;
	UInt8	iModIndex = 0;
	//---Register in Controls
	//-Get dxScancode
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &dxScancode)) {
		DebugCC(5,"Cmd_DisableKey_Replacing_Execute`Failed arg extraction");
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
	DebugCC(5,"Cmd_DisableKey_Replacing_Execute`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(DisableKey_Replacing, "Disables a key and registers a disable", 0, 1, kParams_OneInt)
//### EnableKey_Replacing
bool Cmd_EnableKey_Replacing_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, UInt32 arg3, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
{
	DebugCC(5,"Cmd_EnableKey_Replacing_Execute`Open");
	UInt32	dxScancode = 0;
	UInt8	iModIndex = 0;
	//---Register in Controls
	//-Get dxScancode
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &dxScancode)) {
		DebugCC(5,"Cmd_EnableKey_Replacing_Execute`Failed arg extraction");
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
		DebugCC(5, "Cmd_EnableKey_Replacing_Execute`EnablingKey");
		EnableKey_OriginalExecute(PASS_COMMAND_ARGS);
	}
	else
	{
		DebugCC(5, "Cmd_EnableKey_Replacing_Execute`Neglecting to enable key.");
	}
	DebugCC(5,"Cmd_EnableKey_Replacing_Execute`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(EnableKey_Replacing, "Unregisters disable of this mod. Enables key if there are no disables registered.", 0, 1, kParams_OneInt)
#pragma endregion
#pragma region LoadEvent
extern "C" {
bool OBSEPlugin_Query(const OBSEInterface * obse, PluginInfo * info)
{
	DebugCC(5,"Query`Open");
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

	DebugCC(5, "Query`Close");
	return true;
}
bool OBSEPlugin_Load(const OBSEInterface * obse)
{
	DebugCC(4, "Load`Open");
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
	obse->RegisterCommand(&kCommandInfo_TestControlToString);
	obse->RegisterCommand(&kCommandInfo_TestControlsFromString);

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

	DebugCC(4, "Load`Close");
	return true;
}
};
#pragma endregion
