#pragma once
#include "obse/PluginAPI.h"
#include "obse/CommandTable.h"
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
#include <map>
#include "ControlCollection.h"

#if OBLIVION
extern OBSECommandTableInterface * g_commandTableIntfc; // assigned in OBSEPlugin_Load
extern OBSEScriptInterface * g_scriptIntfc; //For command argument extraction
#define ExtractArgsEx(...) g_scriptIntfc->ExtractArgsEx(__VA_ARGS__)
#define ExtractFormatStringArgs(...) g_scriptIntfc->ExtractFormatStringArgs(__VA_ARGS__)
#endif

extern Cmd_Execute DisableKey_OriginalExecute; //Execute of replaced DisableKey command
extern Cmd_Execute EnableKey_OriginalExecute; //Execute of replaced EnableKey command
extern ControlCollection Controls;

extern Script* pBlankScript; // For use in ExecuteCommand
extern ScriptEventList * pBlankScriptEventList; // For use in ExecuteCommand
extern OBSESerializationInterface	* g_serialization;
extern PluginHandle				g_pluginHandle;

extern const CommandInfo* GetControl_CmdInfo;
extern const CommandInfo* GetAltControl2_CmdInfo;
extern const CommandInfo* ResolveModIndex_CmdInfo;
extern const CommandInfo* IsKeyPressed3_CmdInfo;

extern bool bOblivionLoaded; //Because there is no OblivionLoaded event, this variable is set true later than it should be. (without a helper mod)
typedef bool(*_IsMenuMode)(void);
const _IsMenuMode IsMenuMode = (_IsMenuMode)0x00578F60;

extern OBSEArrayVarInterface* g_arrayIntfc;
typedef OBSEArrayVarInterface::Array	OBSEArray;
typedef OBSEArrayVarInterface::Element	OBSEElement;
