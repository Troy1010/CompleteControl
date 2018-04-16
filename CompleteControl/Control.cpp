#include "Control.h"


Control::Control(int _FormID,
		int _dxScancode,
		int _DisableCount,
		int _OverrideCount,
		MenuModeType __MenuModeType)
{
	FormID = _FormID;
	dxScancode = _dxScancode;
	DisableCount = _DisableCount;
	OverrideCount = _OverrideCount;
	_MenuModeType = __MenuModeType;
}


Control::~Control(void)
{
}
