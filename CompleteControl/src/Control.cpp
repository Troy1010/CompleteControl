#include "Control.h"
#include <vector>
#include "TM_CommonCPP/Narrate.h"
#include "TM_CommonCPP_NarrateOverloads.h"

Control::Control(int _dxScancode, UInt32 _ControlID)
{
	dxScancode = _dxScancode;
	cModIndices_Disables = std::set<UInt8>();
	ControlID = _ControlID;
}

std::string Control::Narrate()
{
	std::ostringstream vReturning;
	vReturning << "dxScancode:" << dxScancode << "\n" << TM_CommonCPP::Narrator::Indent() <<"cModIndices:" << TM_CommonCPP::Narrate(cModIndices_Disables);
	return vReturning.str();
}

Control::~Control(void)
{
}