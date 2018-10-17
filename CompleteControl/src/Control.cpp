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
	vReturning << "Control..";
	TMC::Narrator::iIndent++;
	vReturning << "\n" << TMC::Narrator::Indent() << "dxScancode:" << dxScancode << "\n" << TMC::Narrator::Indent() << "cModIndices:" << TMC::Narrate(cModIndices_Disables);
	TMC::Narrator::iIndent--;
	return vReturning.str();
}

Control::~Control(void)
{
}