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
#include "Misc.h"

//bool Cmd_CommandTemplate_Execute(COMMAND_ARGS);
extern CommandInfo(kCommandInfo_CommandTemplate);
//bool Cmd_DisableKey_Replacing_Execute(COMMAND_ARGS);
extern CommandInfo(kCommandInfo_DisableKey_Replacing);
//bool Cmd_EnableKey_Replacing_Execute(COMMAND_ARGS);
extern CommandInfo(kCommandInfo_EnableKey_Replacing);
//bool Cmd_DisableControl_Replacing_Execute(COMMAND_ARGS);
extern CommandInfo(kCommandInfo_DisableControl_Replacing);
//bool Cmd_EnableControl_Replacing_Execute(COMMAND_ARGS);
extern CommandInfo(kCommandInfo_EnableControl_Replacing);
//bool Cmd_RegisterControl_Replacing_Execute(COMMAND_ARGS);
extern CommandInfo(kCommandInfo_RegisterControl);
