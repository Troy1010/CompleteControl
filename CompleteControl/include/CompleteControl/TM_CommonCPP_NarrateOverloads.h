#pragma once

#include "TM_CommonCPP/Narrate.h"
#include "Control.h"

namespace TMC
{
	std::string Narrate(Control Control);
	std::string Narrate(std::map<int, Control> Controls);
	std::string Narrate(std::set<UInt8> cSet);
	std::string Narrate(UInt8 vUInt8);
	std::string Narrate(UInt32 vUInt32);
	std::string Narrate(std::pair<int, Control> vPair);
}