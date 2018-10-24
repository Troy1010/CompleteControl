#include "Control.h"
#include <vector>
#include "TM_CommonCPP/Misc.h"
#include "TM_CommonCPP/Narrate.h"
#include "TM_CommonCPP_NarrateOverloads.h"

Control::Control(int _dxScancode, UInt32 _ControlID)
{
	dxScancode = _dxScancode;
	cModIndices_Disables = std::set<UInt8>();
	ControlID = _ControlID;
}

Control::Control(std::string sString)
{
	std::vector<std::string> cStrings = TMC::SplitString(sString, ",");
	dxScancode = TMC::IntFromString(cStrings[0]);
	cModIndices_Disables = std::set<UInt8>();
	for (std::string s : TMC::SplitString(cStrings[1], ":"))
	{
		if (s.empty()) continue;
		cModIndices_Disables.insert(TMC::IntFromString(s));
	}
	ControlID = TMC::IntFromString(cStrings[2]);
}

std::string Control::ToString()
{
	std::stringstream ss;
	ss << dxScancode;
	ss << ",";
	for (UInt8 iInt : cModIndices_Disables)
	{
		ss << ":" << iInt;
	}
	ss << "," << ControlID;
	return ss.str();
}

std::string Control::Narrate()
{
	std::ostringstream vReturning;
	vReturning << "Control..";
	TMC::Narrator::iIndent++;
	vReturning <<
		"\n" << TMC::Narrator::Indent() << "dxScancode:" << dxScancode << 
		"\n" << TMC::Narrator::Indent() << "cModIndices:" << TMC::Narrate(cModIndices_Disables) <<
		"\n" << TMC::Narrator::Indent() << "ControlID:" << ControlID;
	TMC::Narrator::iIndent--;
	return vReturning.str();
}

Control::~Control(void)
{
}