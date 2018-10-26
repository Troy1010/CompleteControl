#pragma once
#include "Control.h"
#include "TM_CommonCPP/Misc.h"

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

#include "Misc.h"
#include "Globals.h"
#include "DebugCC.h"

bool Cmd_HandleOblivionLoaded_Execute(COMMAND_ARGS);
extern CommandInfo(kCommandInfo_HandleOblivionLoaded);
void Handler_Save(void * reserved);
void Handler_Load(void * reserved);
void Handler_Preload(void * reserved);
void Handler_NewGame(void * reserved);
