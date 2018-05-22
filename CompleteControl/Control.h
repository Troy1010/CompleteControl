#pragma once
#include "TM_CommonCPP/Properties.h"
#include <string>

class Control
{
private:
	// Members
	static int FreeFormID;
	static bool DoOnce;
public:
	//
	enum MenuModeType
	{
		Both = 0,
		GameMode,
		MenuMode,
	};
	// Constructors
	Control();
	Control(int _FormID, int _dxScancode, int _DisableCount, int _OverrideCount, MenuModeType __MenuModeType);
	~Control(void);
	// Functions
	static int GenerateFormID();

	// Members
	int FormID; //**This has the wrong type
	int dxScancode;
	int DisableCount; //**This has the wrong type
	int OverrideCount; //**This has the wrong type
	MenuModeType _MenuModeType;
	//OnControlDownStringmap
};
