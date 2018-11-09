#pragma once
#include <string>
#include <set>
//#include "DebugCC.h"
#include "Control.h"
#include <map>
#include <vector>

class ControlCollection
{
private:
	std::pair<UInt32, Control> KeyValuePair(Control& vControl);
public:
	//---Constructors
	ControlCollection();
	ControlCollection(std::string const& s);
	//---
	std::string Stringize();
	void EnableAll();
	void SetOutcomes();
	void RegisterVanillaControls();
	Control* FindByScancode(int iDXScancode);
	Control* FindByID(UInt32 vControlID);
	//---
	std::map<UInt32, Control> Items;
};

