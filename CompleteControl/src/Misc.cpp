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

Control* GetControlByScancode(int iDXScancode)
{
	for (Control &vControl : Controls)
	{
		if (vControl.GetDXScancode() == iDXScancode)
		{
			return &vControl;
		}
	}
	return NULL;
}

Control* GetControlByID(UInt32 vControlID)
{
	for (Control &vControl : Controls)
	{
		if (vControl.ControlID == vControlID)
		{
			return &vControl;
		}
	}
	return NULL;
}

bool IsDisabled(Control vControl)
{
	return !vControl.cModIndices_Disables.empty();
}

void SetOutcomeForAllControls(std::vector<Control> cControls)
{
	DebugCC(5, std::string(__func__) + "`Open");
	for (auto vControl : cControls)
	{
		vControl.SetOutcome();
	}
	DebugCC(5, std::string(__func__) + "`Close");
}

std::string StringizeControls(std::vector<Control> cControls)
{
	std::stringstream ss;
	for (auto vControl : cControls)
	{
		ss << "`" << vControl.ToString();
	}
	return ss.str();
}

std::vector<Control> ControlsFromString(std::string sBigString)
{
	std::vector<Control> cReturningControls;
	for (auto s : TMC::SplitString(sBigString, "`"))
	{
		if (s.empty()) { continue; }
		cReturningControls.push_back(Control(s));
	}
	return cReturningControls;
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

std::vector<Control> InitializeControls()
{
	DebugCC(5, std::string(__func__) + "`Open");
	std::vector<Control> cControls;
	for (int i = 0; i < Control::VanillaControlID_EnumSize; ++i)
	{
		cControls.push_back(Control(i));
	}
	DebugCC(6, std::string(__func__) + "`cControls:" + TMC::Narrate(cControls));
	DebugCC(5, std::string(__func__) + "`Close");
	return cControls;
}

