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
#include "ExecuteCommand.h"

extern CommandInfo(kCommandInfo_DisableKey_Replacing);
extern CommandInfo(kCommandInfo_EnableKey_Replacing);
extern CommandInfo(kCommandInfo_DisableControl_Replacing);
extern CommandInfo(kCommandInfo_EnableControl_Replacing);
extern CommandInfo(kCommandInfo_RegisterControl);
extern CommandInfo(kCommandInfo_IsDisabled);
extern CommandInfo(kCommandInfo_GetKey);
extern CommandInfo(kCommandInfo_UnreportedDisable);
extern CommandInfo(kCommandInfo_UnreportedEnable);
extern CommandInfo(kCommandInfo_IsEngaged);
extern CommandInfo(kCommandInfo_IsEngaged_ByRef);
extern CommandInfo(kCommandInfo_OnControlDown2);
extern CommandInfo(kCommandInfo_OnControlDown2_ByRef);
