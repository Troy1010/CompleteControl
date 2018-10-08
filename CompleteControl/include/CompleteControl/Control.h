#pragma once
#include "TM_CommonCPP/Properties.h"
#include <string>
#include <vector>
#include <set>

class Control
{
public:
	//---Constructors
	Control(int _dxScancode);
	~Control(void);
	//---Members
	int dxScancode;
	std::set<UInt8> cModIndices;
	std::string Narrate();
};
