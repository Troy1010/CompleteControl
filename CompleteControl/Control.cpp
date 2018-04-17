#include "Control.h"

Control::Control()
{
	FormID = 0;
	dxScancode = 0;
	DisableCount = 0;
	OverrideCount = 0;
	_MenuModeType = MenuModeType::Both;
	if (!DoOnce)
	{
		DoOnce = true;
		FreeFormID = 1;
	}
}

Control::Control(int _FormID, int _dxScancode, int _DisableCount, int _OverrideCount, MenuModeType __MenuModeType) //What are these variables usually prefixed with? _? Then what about __MenuModeType?
{
	FormID = _FormID;
	dxScancode = _dxScancode;
	DisableCount = _DisableCount;
	OverrideCount = _OverrideCount;
	_MenuModeType = __MenuModeType;
	FreeFormID = 1;
}


Control::~Control(void)
{
}

int Control::GenerateFormID(void)
{
	int _int;
	_int = FreeFormID;
	FreeFormID++;
	return _int;
}
