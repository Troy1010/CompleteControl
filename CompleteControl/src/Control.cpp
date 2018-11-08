#include "Control.h"
#include <vector>
#include "TM_CommonCPP/Misc.h"
#include "TM_CommonCPP/Narrate.h"
#include "TM_CommonCPP_NarrateOverloads.h"
#include <sstream>
#include "ExecuteCommand.h"
#include "Globals.h"

const bool Control::IsEngaged()
{
	return IsPressed() && !IsDisabled();
}

const bool Control::IsPressed()
{
	return ExecuteCommand(IsKeyPressed3_CmdInfo, GetDXScancode());
}

const bool Control::IsDisabled()
{
	return !cModIndices_Disables.empty();
}

int Control::GetDXScancode() //perhaps I could remake this to go faster
{
	DebugCC(7, std::string(__func__) + "`Open. ControlID:" + TMC::Narrate(ControlID));
	if (ControlID < VanillaControlID_EnumSize)
	{
		int dxScancode = ExecuteCommand(GetControl_CmdInfo, ControlID);
		if (dxScancode == 0xFF)
		{
			dxScancode = ExecuteCommand(GetAltControl2_CmdInfo, ControlID);
			DebugCC(7, "dxScancode was 0xFF! Tryagain:" + TMC::Narrate(dxScancode));
		}
		DebugCC(7, std::string(__func__) + "`Close. dxScancode:" + TMC::Narrate(dxScancode));
		return dxScancode;
	}
	else
	{
		DebugCC(7, std::string(__func__) + "`Close. dxScancode:" + TMC::Narrate(dxScancode_NonVanilla));
		return dxScancode_NonVanilla;
	}
}

void Control::ResolveModIndices()
{
	UInt8 iNewModIndex;
	if (ControlID >= VanillaControlID_EnumSize)
	{
		DebugCC(7, std::string(__func__) + "`ControlID OLD:" + TMC::Narrate(ControlID));
		iNewModIndex = ExecuteCommand(ResolveModIndex_CmdInfo, ControlID >> 24); // iModIndex:ControlID >> 24
		if (iNewModIndex != 0xFF)
		{
			ControlID &= ~UInt32(0xFF000000); // Clear out whatever is at the first two hex digits.
			DebugCC(7, std::string(__func__) + "`ControlID CLEARED:" + TMC::Narrate(ControlID));
			ControlID |= UInt32(iNewModIndex) << 24; // Apply mask.
			DebugCC(7, std::string(__func__) + "`ControlID NEW:" + TMC::Narrate(ControlID));
		}
		else
		{
			DebugCC(7, "Controls BEFORE:" + TMC::Narrate(Controls));
			bDeleteMe = true; // Ugly
			Controls.erase(std::remove_if(Controls.begin(), Controls.end(), [](const Control & o) { return o.bDeleteMe; }), Controls.end()); // Dangerous, Ugly. Why?
			DebugCC(7, "Controls AFTER:" + TMC::Narrate(Controls));
			return;
		}
	}
	//-cModIndices_Disables
	decltype(cModIndices_Disables) cModIndices_Disables_NEW;
	for (auto iModIndex : cModIndices_Disables)
	{
		DebugCC(6, std::string(__func__) + "`iModIndexBEFORE:" + TMC::Narrate(iModIndex));
		iNewModIndex = ExecuteCommand(ResolveModIndex_CmdInfo, iModIndex);
		DebugCC(6, std::string(__func__) + "`iModIndexAFTER:" + TMC::Narrate(iNewModIndex));
		if (iNewModIndex != 255)
		{
			cModIndices_Disables_NEW.insert(iNewModIndex);
		}
	}
	cModIndices_Disables = cModIndices_Disables_NEW;
	//-cModIndices_UnreportedDisables
	decltype(cModIndices_UnreportedDisables) cModIndices_UnreportedDisables_NEW;
	for (auto iModIndex : cModIndices_UnreportedDisables)
	{
		DebugCC(6, std::string(__func__) + "`iModIndexBEFORE:" + TMC::Narrate(iModIndex));
		iNewModIndex = ExecuteCommand(ResolveModIndex_CmdInfo, iModIndex);
		DebugCC(6, std::string(__func__) + "`iModIndexAFTER:" + TMC::Narrate(iNewModIndex));
		if (iNewModIndex != 255)
		{
			cModIndices_UnreportedDisables_NEW.insert(iNewModIndex);
		}
	}
	cModIndices_UnreportedDisables = cModIndices_UnreportedDisables_NEW;
}

void Control::SetOutcome()
{
	if (eMenuModeType == MenuModeType::Both || (IsMenuMode() && eMenuModeType == MenuModeType::MenuMode) || (!IsMenuMode() && eMenuModeType == MenuModeType::GameMode))
	{
		if (this->IsDisabled())
		{
			ExecuteCommand(DisableKey_OriginalExecute, this->GetDXScancode());
		}
		else
		{
			ExecuteCommand(EnableKey_OriginalExecute, this->GetDXScancode());
		}
	}
}


Control::Control(UInt32 _ControlID, MenuModeType _eMenuModeType = MenuModeType::GameMode, int _dxScancode_NonVanilla = 0)
{
	ControlID = _ControlID;
	eMenuModeType = _eMenuModeType;
	dxScancode_NonVanilla = _dxScancode_NonVanilla;
	cModIndices_Disables = std::set<UInt8>();
	cModIndices_UnreportedDisables = std::set<UInt8>();
	cScriptRefs_ReceivedOnControlDown = std::set<UInt32>();
	bDeleteMe = false;
}
Control::Control(UInt32 _ControlID)
{
	ControlID = _ControlID;
	eMenuModeType = MenuModeType::GameMode;
	dxScancode_NonVanilla = 0;
	cModIndices_Disables = std::set<UInt8>();
	cModIndices_UnreportedDisables = std::set<UInt8>();
	cScriptRefs_ReceivedOnControlDown = std::set<UInt32>();
	bDeleteMe = false;
}
Control::Control(std::string sString)
{
	std::vector<std::string> cStrings = TMC::SplitString(sString, ",");
	DebugCC(7,"cStrings:" + TMC::Narrate(cStrings));
	ControlID = TMC::IntFromString(cStrings[0]);
	eMenuModeType = Control::MenuModeType(TMC::IntFromString(cStrings[1]));
	dxScancode_NonVanilla = TMC::IntFromString(cStrings[2]);
	cModIndices_Disables = std::set<UInt8>();
	for (std::string s : TMC::SplitString(cStrings[3], ":"))
	{
		if (s.empty()) continue;
		cModIndices_Disables.insert(TMC::IntFromString(s));
	}
	cModIndices_UnreportedDisables = std::set<UInt8>();
	for (std::string s : TMC::SplitString(cStrings[4], ":"))
	{
		if (s.empty()) continue;
		cModIndices_UnreportedDisables.insert(TMC::IntFromString(s));
	}
	cScriptRefs_ReceivedOnControlDown = std::set<UInt32>();
	bDeleteMe = false;
}

std::string Control::ToString()
{
	DebugCC(7, std::string(__func__) + "`Controls:" + TMC::Narrate(Controls));
	std::stringstream ss;
	ss << ControlID;
	ss << "," << eMenuModeType;
	ss << "," << dxScancode_NonVanilla;
	ss << ",";
	for (UInt8 iInt : cModIndices_Disables)
	{
		ss << ":" << int(iInt); // iInt requires a cast to int for ss to interpret it correctly.
	}
	ss << ",";
	for (UInt8 iInt : cModIndices_UnreportedDisables)
	{
		ss << ":" << int(iInt); // iInt requires a cast to int for ss to interpret it correctly.
	}
	DebugCC(7, std::string(__func__) + "`ss.str():" + ss.str());
	return ss.str();
}

std::string Control::Narrate()
{
	std::ostringstream vReturning;
	vReturning << "Control..";
	TMC::Narrator::iIndent++;
	vReturning <<
		"\n" << TMC::Narrator::Indent() << "ControlID:" << ControlID <<
		"\n" << TMC::Narrator::Indent() << "dxScancode:" << GetDXScancode() <<
		"\n" << TMC::Narrator::Indent() << "eMenuModeType:" << eMenuModeType <<
		"\n" << TMC::Narrator::Indent() << "cModIndices:" << TMC::Narrate(cModIndices_Disables) <<
		"\n" << TMC::Narrator::Indent() << "cModIndices_Unregistered:" << TMC::Narrate(cModIndices_UnreportedDisables);
	TMC::Narrator::iIndent--;
	return vReturning.str();
}

Control::~Control(void)
{
}