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

#include "DebugCC.h"
#include "CopyPastedFromOBSE.h"
#include "Misc.h"
#include "ExecuteCommand.h"

// extern CommandInfo(kCommandInfo_DisableKey2);
// extern CommandInfo(kCommandInfo_EnableKey2);
// extern CommandInfo(kCommandInfo_DisableControl2);
// extern CommandInfo(kCommandInfo_DisableControl_ByRef);
// extern CommandInfo(kCommandInfo_DisableControls);
// extern CommandInfo(kCommandInfo_EnableControl2);
// extern CommandInfo(kCommandInfo_EnableControl_ByRef);
// extern CommandInfo(kCommandInfo_EnableControls);
// extern CommandInfo(kCommandInfo_RegisterControl);
// extern CommandInfo(kCommandInfo_IsDisabled);
// extern CommandInfo(kCommandInfo_IsDisabled_ByRef);
// extern CommandInfo(kCommandInfo_GetKey);
// extern CommandInfo(kCommandInfo_GetKey_ByRef);
// extern CommandInfo(kCommandInfo_UnreportedDisable);
// extern CommandInfo(kCommandInfo_UnreportedDisable_ByRef);
// extern CommandInfo(kCommandInfo_UnreportedEnable);
// extern CommandInfo(kCommandInfo_UnreportedEnable_ByRef);
// extern CommandInfo(kCommandInfo_IsEngaged);
// extern CommandInfo(kCommandInfo_IsEngaged_ByRef);
// extern CommandInfo(kCommandInfo_IsPressed);
// extern CommandInfo(kCommandInfo_IsPressed_ByRef);
// extern CommandInfo(kCommandInfo_OnControlDown2);
// extern CommandInfo(kCommandInfo_OnControlDown2_ByRef);
