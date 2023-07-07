#include "Control.h"
#include <vector>
#include "TM_CommonCPP/ToLogStr.h"
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

const bool Control::IsDisabled() //ShouldBeDisabled might be a more appropriate name.
{
	return !cModIndices_Disables.empty();
}

int Control::GetDXScancode() //perhaps I could remake this to go faster
{
	Logv(std::string(__func__) + "`Open. ControlID:" + TMC::ToLogStr(ControlID));
	if (ControlID < VanillaControlID_EnumSize)
	{
		int dxScancode = ExecuteCommand(GetControl_CmdInfo, ControlID);
		if (dxScancode == 0xFF)
		{
			dxScancode = ExecuteCommand(GetAltControl2_CmdInfo, ControlID);
			Logv("dxScancode was 0xFF! Tryagain:" + TMC::ToLogStr(dxScancode));
		}
		Logv(std::string(__func__) + "`Close. dxScancode:" + TMC::ToLogStr(dxScancode));
		return dxScancode;
	}
	else
	{
		Logv(std::string(__func__) + "`Close. dxScancode:" + TMC::ToLogStr(dxScancode_NonVanilla));
		return dxScancode_NonVanilla;
	}
}

void Control::ResolveModIndices()
{
	UInt8 iNewModIndex;
	if (ControlID >= VanillaControlID_EnumSize)
	{
		Logv(std::string(__func__) + "`ControlID OLD:" + TMC::ToLogStr(ControlID));
		iNewModIndex = ExecuteCommand(ResolveModIndex_CmdInfo, ControlID >> 24); // iModIndex:ControlID >> 24
		if (iNewModIndex != 0xFF)
		{
			ControlID &= ~UInt32(0xFF000000); // Clear out whatever is at the first two hex digits.
			Logv(std::string(__func__) + "`ControlID CLEARED:" + TMC::ToLogStr(ControlID));
			ControlID |= UInt32(iNewModIndex) << 24; // Apply mask.
			Logv(std::string(__func__) + "`ControlID NEW:" + TMC::ToLogStr(ControlID));
		}
		else
		{
			Logv("Controls BEFORE:" + TMC::ToLogStr(Controls));
			Controls.Items.erase(ControlID);
			Logv("Controls AFTER:" + TMC::ToLogStr(Controls));
			return;
		}
	}
	//-cModIndices_Disables
	decltype(cModIndices_Disables) cModIndices_Disables_NEW;
	for (auto iModIndex : cModIndices_Disables)
	{
		Logv(std::string(__func__) + "`iModIndexBEFORE:" + TMC::ToLogStr(iModIndex));
		iNewModIndex = ExecuteCommand(ResolveModIndex_CmdInfo, iModIndex);
		Logv(std::string(__func__) + "`iModIndexAFTER:" + TMC::ToLogStr(iNewModIndex));
		if (iNewModIndex != 255)
		{
			cModIndices_Disables_NEW.insert(iNewModIndex);
		}
	}
	cModIndices_Disables = cModIndices_Disables_NEW; //leaking?
	//-cModIndices_UnreportedDisables
	decltype(cModIndices_UnreportedDisables) cModIndices_UnreportedDisables_NEW;
	for (auto iModIndex : cModIndices_UnreportedDisables)
	{
		Logv(std::string(__func__) + "`iModIndexBEFORE:" + TMC::ToLogStr(iModIndex));
		iNewModIndex = ExecuteCommand(ResolveModIndex_CmdInfo, iModIndex);
		Logv(std::string(__func__) + "`iModIndexAFTER:" + TMC::ToLogStr(iNewModIndex));
		if (iNewModIndex != 255)
		{
			cModIndices_UnreportedDisables_NEW.insert(iNewModIndex);
		}
	}
	cModIndices_UnreportedDisables = cModIndices_UnreportedDisables_NEW; //leaking?
}

void Control::SetOutcome()
{
	if (eMenuModeType == MenuModeType::Both || (IsMenuMode() && eMenuModeType == MenuModeType::MenuMode) || (!IsMenuMode() && eMenuModeType == MenuModeType::GameMode))
	{
		if (this->IsDisabled() || !cModIndices_UnreportedDisables.empty())
		{
			ExecuteCommand(DisableKey_OriginalExecute, this->GetDXScancode());
		}
		else
		{
			ExecuteCommand(EnableKey_OriginalExecute, this->GetDXScancode());
		}
	}
}

void Control::Enable()
{
	ExecuteCommand(EnableKey_OriginalExecute, this->GetDXScancode());
}

Control::Control()
{
	Logw("Warning: Control's default constructor was called.")
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
	std::vector<std::string> cStrings = TMC::Str::Split(sString, ",");
	Logv("cStrings:" + TMC::ToLogStr(cStrings));
	ControlID = std::stoi(cStrings[0]);
	eMenuModeType = Control::MenuModeType(std::stoi(cStrings[1]));
	dxScancode_NonVanilla = std::stoi(cStrings[2]);
	cModIndices_Disables = std::set<UInt8>();
	for (std::string s : TMC::Str::Split(cStrings[3], ":"))
	{
		if (s.empty()) continue;
		cModIndices_Disables.insert(std::stoi(s));
	}
	cModIndices_UnreportedDisables = std::set<UInt8>();
	for (std::string s : TMC::Str::Split(cStrings[4], ":"))
	{
		if (s.empty()) continue;
		cModIndices_UnreportedDisables.insert(std::stoi(s));
	}
	cScriptRefs_ReceivedOnControlDown = std::set<UInt32>();
	bDeleteMe = false;
}

std::string Control::ToString()
{
	Logv(std::string(__func__) + "`Controls:" + TMC::ToLogStr(Controls));
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
	Logv(std::string(__func__) + "`ss.str():" + ss.str());
	return ss.str();
}

std::string Control::Narrate()
{
	std::ostringstream vReturning;
	vReturning << "Control..";
	TMC::LogStrFactory::iIndent++;
	vReturning <<
		"\n" << TMC::LogStrFactory::Indent() << "ControlID:" << ControlID <<
		"\n" << TMC::LogStrFactory::Indent() << "dxScancode:" << GetDXScancode() <<
		"\n" << TMC::LogStrFactory::Indent() << "eMenuModeType:" << eMenuModeType <<
		"\n" << TMC::LogStrFactory::Indent() << "cModIndices:" << TMC::ToLogStr(cModIndices_Disables) <<
		"\n" << TMC::LogStrFactory::Indent() << "cModIndices_Unregistered:" << TMC::ToLogStr(cModIndices_UnreportedDisables);
	TMC::LogStrFactory::iIndent--;
	return vReturning.str();
}

Control::~Control(void)
{
}