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
#include "TM_CommonCPP/Misc.h"
#include "TM_CommonCPP/Narrate.h"
#include "TM_CommonCPP/StdStringFromFormatString.h"
#include "TM_CommonCPP_NarrateOverloads.h"
#include "obse/Script.h"
#include "obse/Hooks_DirectInput8Create.h"
#include <sstream>

#include "DebugCC.h"
#include "CopyPastedFromOBSE.h"


//### TestControlsFromString
bool Cmd_TestControlsFromString_Execute(COMMAND_ARGS);
extern CommandInfo(kCommandInfo_TestControlsFromString);
//### TestControlToString
bool Cmd_TestControlToString_Execute(COMMAND_ARGS);
extern CommandInfo(kCommandInfo_TestControlToString);
//### PrintControls
bool Cmd_PrintControls_Execute(COMMAND_ARGS);
extern CommandInfo(kCommandInfo_PrintControls);
//### TestCeil
bool Cmd_TestCeil_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, UInt32 arg3, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr);
extern CommandInfo(kCommandInfo_TestCeil);
//### BasicRuntimeTests
bool Cmd_BasicRuntimeTests_Execute(COMMAND_ARGS);
extern CommandInfo(kCommandInfo_BasicRuntimeTests);
//### TestGetControlDirectly
bool Cmd_TestGetControlDirectly_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, UInt32 arg3, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr);
extern CommandInfo(kCommandInfo_TestGetControlDirectly);
//### TestGetControlDirectly2
bool Cmd_TestGetControlDirectly2_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, UInt32 arg3, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr);
extern CommandInfo(kCommandInfo_TestGetControlDirectly2);
//### TestGetControlCopyPasta
bool Cmd_TestGetControlCopyPasta_Execute(COMMAND_ARGS);
extern CommandInfo(kCommandInfo_TestGetControlCopyPasta);
//### TestDisableKeyCopyPasta
bool Cmd_TestDisableKeyCopyPasta_Execute(COMMAND_ARGS);
extern CommandInfo(kCommandInfo_TestDisableKeyCopyPasta);
//### GenerateEnum
bool Cmd_GenerateEnum_Execute(COMMAND_ARGS);
extern CommandInfo(kCommandInfo_GenerateEnum);
