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

const bool Contains(std::set<UInt32> cSet, UInt32 vItem);
const bool Contains(std::set<UInt8> cSet, UInt8 vItem);
Control* GetControlByScancode(int iDXScancode);
Control* GetControlByID(UInt32 vControlID);
void SetOutcomeForAllControls(std::vector<Control> cControls);
std::string StringizeControls(std::vector<Control> cControls);
std::vector<Control> ControlsFromString(std::string sBigString);
void SafeConsolePrint(std::string sString);
std::vector<Control> InitializeControls();

static ParamInfo kParams_ThreeInts[3] =
{
	{	"int", kParamType_Integer, 0 },
	{	"int", kParamType_Integer, 0 },
	{	"int", kParamType_Integer, 0 },
};

static ParamInfo kParams_OneRefTwoInts[3] =
{
	{	"int", kParamType_InventoryObject, 0},
	{	"int", kParamType_Integer, 0 },
	{	"int", kParamType_Integer, 0 },
};
