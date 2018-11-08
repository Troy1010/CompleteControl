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


extern CommandInfo(kCommandInfo_CommandTemplate);
extern CommandInfo(kCommandInfo_Test1);
extern CommandInfo(kCommandInfo_TestControlsFromString);
extern CommandInfo(kCommandInfo_TestControlToString);
extern CommandInfo(kCommandInfo_PrintControls);
extern CommandInfo(kCommandInfo_TestCeil);
extern CommandInfo(kCommandInfo_BasicRuntimeTests);
extern CommandInfo(kCommandInfo_TestGetControlDirectly);
extern CommandInfo(kCommandInfo_TestGetControlDirectly2);
extern CommandInfo(kCommandInfo_TestGetControlCopyPasta);
extern CommandInfo(kCommandInfo_TestDisableKeyCopyPasta);
extern CommandInfo(kCommandInfo_GenerateEnum);
