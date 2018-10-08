#pragma once
#include "TM_CommonCPP/Properties.h"
#include <string>
#include <vector>
#include <set>

class Control
{
public:
	//
	enum MenuModeType
	{
		Both = 0,
		GameMode,
		MenuMode,
	};
	//---Constructors
	Control(int _dxScancode);
	~Control(void);
	//---Members
	int dxScancode;
	std::set<UInt8> cModIndices_Disables;
	std::string Narrate();

	std::set<UInt8> cModIndices_UnreportedDisables;
	MenuModeType eMenuModeType;
	UInt32 ControlID;
};
