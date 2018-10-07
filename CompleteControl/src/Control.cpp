#include "Control.h"
#include <vector>

Control::Control(int _dxScancode)
{
	dxScancode = _dxScancode;
	UInt8 cTemp[] = { 1,3,5 };
	//std::set<UInt8> cModIndices(cTemp.begin(), cTemp.end());
	//std::set<int> seet = { 10, 20, 30, 40, 50 };
}


Control::~Control(void)
{
}