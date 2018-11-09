#include "Misc.h"
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

#include "Globals.h"
#include "Settings.h"
#include "DebugCC.h"
#include "ExecuteCommand.h"

const bool Contains(std::set<UInt32> cSet, UInt32 vItem)
{
	return cSet.find(vItem) != cSet.end();
}
const bool Contains(std::set<UInt8> cSet, UInt8 vItem)
{
	return cSet.find(vItem) != cSet.end();
}

Control* GetControlByScancode(int iDXScancode)
{
	for (auto &vControl : Controls)
	{
		if (vControl.second.GetDXScancode() == iDXScancode)
		{
			return &vControl.second;
		}
	}
	return NULL;
}

Control* GetControlByID(UInt32 vControlID) //Could definately help to make this faster
{
	for (auto &vControl : Controls)
	{
		if (vControl.second.ControlID == vControlID)
		{
			return &vControl.second;
		}
	}
	return NULL;
}

bool IsDisabled(Control vControl)
{
	return !vControl.cModIndices_Disables.empty();
}

void SetOutcomeForAllControls(std::map<int, Control> cControls)
{
	DebugCC(5, std::string(__func__) + "`Open");
	for (auto vControl : cControls)
	{
		vControl.second.SetOutcome();
	}
	DebugCC(5, std::string(__func__) + "`Close");
}

std::string StringizeControls(std::map<int,Control> cControls)
{
	std::stringstream ss;
	for (auto vControl : cControls)
	{
		ss << "`" << vControl.second.ToString();
	}
	return ss.str();
}

void SafeConsolePrint(std::string sString)
{
	if (!bOblivionLoaded) // Trying to print to console without Oblivion loaded causes CTD.
	{
		//do nothing
	}
	else if (sString.length() < 1000) //1000 might not be the exact limit. Exceeding limit causes CTD.
	{
		Console_Print(sString.c_str());
	}
	else
	{
		Console_Print(sString.substr(0, 1000).c_str());
		Console_Print("<MessageTooLarge>");
	}
}

std::map<int, Control> InitializeControls()
{
	DebugCC(5, std::string(__func__) + "`Open");
	std::map<int, Control> cControls;
	for (UInt32 i = 0; i < Control::VanillaControlID_EnumSize; ++i)
	{
		auto vControl = Control(i);
		if (i == 17)
		{
			vControl.eMenuModeType = Control::MenuModeType::Both;
		}
		cControls.insert({ vControl.ControlID ,vControl });
	}
	DebugCC(6, std::string(__func__) + "`cControls:" + TMC::Narrate(cControls));
	DebugCC(5, std::string(__func__) + "`Close");
	return cControls;
}

std::map<int, Control> ControlsFromString(std::string sBigString)
{
	std::map<int, Control> cReturningControls;
	for (auto s : TMC::SplitString(sBigString, "`"))
	{
		if (s.empty()) { continue; }
		auto vControl = Control(s);
		cReturningControls.insert({ vControl.ControlID ,vControl });
	}
	return cReturningControls;
}

