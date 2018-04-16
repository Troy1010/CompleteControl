#pragma once
#include "TM_CommonCPP/TM_CommonCPP/Properties.h"

class Control
{
	public:
		enum MenuModeType
		{
			Both = 0, //?Do I need the =0?
			GameMode,
			MenuMode,
		};
		Control(int _FormID, int _dxScancode, int _DisableCount, int _OverrideCount, MenuModeType __MenuModeType);
		~Control(void);

		int FormID; //**This has the wrong type
		int dxScancode;
		int DisableCount; //**This has the wrong type
		int OverrideCount; //**This has the wrong type
		MenuModeType _MenuModeType;
		//OnControlDownStringmap
};
