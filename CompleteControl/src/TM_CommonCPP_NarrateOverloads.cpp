#include "TM_CommonCPP_NarrateOverloads.h"
#include "TM_CommonCPP/Narrate.h"
#include "Control.h"
#include <sstream>

namespace TMC
{
	std::string Narrate(Control Control)
	{
		return Control.Narrate();
	}
	std::string Narrate(std::vector<Control> Controls)
	{
		return Narrator::Narrate_Collection(Controls);
	}
	std::string Narrate(std::map<int, Control> Controls)
	{
		return Narrator::Narrate_Collection(Controls);
	}
	std::string Narrate(std::set<UInt8> cSet)
	{
		return Narrator::Narrate_Collection(cSet);
	}
	std::string Narrate(UInt8 vUInt8)
	{
		return std::to_string(vUInt8);
	}
	std::string Narrate(UInt32 vUInt32)
	{
		return std::to_string(vUInt32);
	}
	std::string Narrate(std::pair<int, Control> vPair)
	{
		return Narrate(vPair.first) + ":" + Narrate(vPair.second);
	}
}
