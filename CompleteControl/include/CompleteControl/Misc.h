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

Control GetControlByScancode(int iDXScancode);
Control GetControlByID(UInt32 vControlID);
void SetOutcomeForAllControls(std::vector<Control> cControls);
std::string StringizeControls(std::vector<Control> cControls);
std::vector<Control> ControlsFromString(std::string sBigString);
void SafeConsolePrint(std::string sString);
std::vector<Control> InitializeControls();
