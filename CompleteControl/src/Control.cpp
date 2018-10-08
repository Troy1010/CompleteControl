#include "Control.h"
#include <vector>
#include "TM_CommonCPP/Narrate.h"
#include "TM_CommonCPP_NarrateOverloads.h"

Control::Control(int _dxScancode)
{
	dxScancode = _dxScancode;
	cModIndices = std::set<UInt8>();
}

std::string Control::Narrate()
{
	std::ostringstream vReturning;
	vReturning << "dxScancode:" << dxScancode << "\n" << TM_CommonCPP::Narrator::Indent() <<"cModIndices:" << TM_CommonCPP::Narrate(cModIndices);
	return vReturning.str();
}

Control::~Control(void)
{
}