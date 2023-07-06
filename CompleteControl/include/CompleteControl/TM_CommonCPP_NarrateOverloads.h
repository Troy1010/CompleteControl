#pragma once

#include "TM_CommonCPP/ToLogStr.h"
#include "Control.h"
#include "ControlCollection.h"

namespace TMC
{
	std::string ToLogStr(Control Control);
	std::string ToLogStr(std::map<UInt32, Control> Controls);
	std::string ToLogStr(std::set<UInt8> cSet);
	std::string ToLogStr(UInt8 vUInt8);
	std::string ToLogStr(UInt32 vUInt32);
	std::string ToLogStr(std::pair<UInt32, Control> vPair);
	std::string ToLogStr(ControlCollection Controls);
}