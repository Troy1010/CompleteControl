#pragma once
#include "TM_CommonCPP/Properties.h"
#include <string>
#include <vector>

class Control
{
public:
	//---Constructors
	Control(int _dxScancode);
	~Control(void);
	//---Members
	int dxScancode;
	std::vector<int> cModIndices;
};
