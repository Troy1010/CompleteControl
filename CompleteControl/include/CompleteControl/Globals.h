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

extern int iIntGlobal;

extern IDebugLog		gLog; //Log
extern Cmd_Execute DisableKey_OriginalExecute; //Execute of replaced DisableKey command
extern Cmd_Execute EnableKey_OriginalExecute; //Execute of replaced EnableKey command
extern const CommandInfo* DisableKey_CmdInfo; //DisableKey command
extern const CommandInfo* GetControl; //GetControl command
extern std::vector<Control> Controls;

extern Script* pBlankScript;
extern ScriptEventList * pBlankScriptEventList;
extern bool bOblivionLoaded;
extern OBSESerializationInterface	* g_serialization;
extern PluginHandle				g_pluginHandle;
