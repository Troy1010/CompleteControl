#pragma once
#include "TM_CommonCPP/Properties.h"

class Control
{
	public:
		enum MenuModeType
		{
			Both = 0, //?Do I need the =0?
			GameMode,
			MenuMode,
		};
		Control(int _FormID);
		~Control(void);

		int FormID; //**This has the wrong type
		int dxScancode;
		int DisableCount;
		int OverrideCount;
		MenuModeType _MenuModeType;
		//OnControlDownStringmap
};
