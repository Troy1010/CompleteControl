#pragma once

#include "TM_CommonCPP/Narrate.h"
#include "Control.h"

namespace TMC
{
	std::string Narrate(std::vector<Control> Controls);
	std::string Narrate(std::set<UInt8> cSet);
	std::string Narrate(UInt8 vUInt8);
	std::string Narrate(UInt32 vUInt32);
}