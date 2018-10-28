#include "Control.h"
#include <vector>
#include "TM_CommonCPP/Misc.h"
#include "TM_CommonCPP/Narrate.h"
#include "TM_CommonCPP_NarrateOverloads.h"
#include <sstream>
#include "ExecuteCommand.h"
#include "Globals.h"


int Control::GetDXScancode()
{
	if (ControlID < VanillaControlID_EnumSize)
	{
		return ExecuteCommand(GetControl_CmdInfo, ControlID);
	}
	else
	{
		return dxScancode_NonVanilla;
	}
}

void Control::ResolveModIndices()
{
	decltype(cModIndices_Disables) cModIndices_Disables_NEW;
	for (auto iModIndex : cModIndices_Disables)
	{
		DebugCC(6, std::string(__func__) + "`iModIndexBEFORE:" + TMC::Narrate(iModIndex));
		UInt8 iNewModIndex = ExecuteCommand(ResolveModIndex_CmdInfo, iModIndex);
		DebugCC(6, std::string(__func__) + "`iModIndexAFTER:" + TMC::Narrate(iNewModIndex));
		if (iNewModIndex != 255)
		{
			cModIndices_Disables_NEW.insert(iNewModIndex);
		}
	}
	cModIndices_Disables = cModIndices_Disables_NEW; // leaking?
}

bool Control::IsDisabled()
{
	return !this->cModIndices_Disables.empty();
}

void Control::SetOutcome()
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


Control::Control(UInt32 _ControlID, int _dxScancode_NonVanilla)
{
	cModIndices_Disables = std::set<UInt8>();
	ControlID = _ControlID;
	dxScancode_NonVanilla = _dxScancode_NonVanilla;
}
Control::Control(UInt32 _ControlID)
{
	cModIndices_Disables = std::set<UInt8>();
	ControlID = _ControlID;
}
Control::Control(std::string sString)
{
	std::vector<std::string> cStrings = TMC::SplitString(sString, ",");
	DebugCC(7,"cStrings:" + TMC::Narrate(cStrings));
	ControlID = TMC::IntFromString(cStrings[0]);
	cModIndices_Disables = std::set<UInt8>();
	for (std::string s : TMC::SplitString(cStrings[1], ":"))
	{
		if (s.empty()) continue;
		cModIndices_Disables.insert(TMC::IntFromString(s));
	}
}

std::string Control::ToString()
{
	DebugCC(7, std::string(__func__) + "`Controls:" + TMC::Narrate(Controls));
	std::stringstream ss;
	ss << ControlID << ",";
	for (UInt8 iInt : cModIndices_Disables)
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
		"\n" << TMC::Narrator::Indent() << "dxScancode:" << this->GetDXScancode() <<
		"\n" << TMC::Narrator::Indent() << "cModIndices:" << TMC::Narrate(cModIndices_Disables);
	TMC::Narrator::iIndent--;
	return vReturning.str();
}

Control::~Control(void)
{
}