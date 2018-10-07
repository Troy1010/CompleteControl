#include "TM_CommonCPP_NarrateOverloads.h"
#include "TM_CommonCPP/Narrate.h"
#include "Control.h"
#include <sstream>

namespace TM_CommonCPP
{
	std::string Narrate(std::vector<Control> Controls)
	{
		std::ostringstream vReturning;
		int i = 0;
		for each (Control vControl in Controls)
		{
			vReturning << "\n\t" << i << ":??" ;
			i++;
		}
		return "NarrateControls(Count:" + Narrate(i) + ").." + vReturning.str();
	}
	std::string Narrate(std::set<UInt8> cSet)
	{
		return Narrator::Narrate_Collection(cSet);
	}
	std::string Narrate(UInt8 vUInt8)
	{
		return Narrator::Narrate_Stringable(vUInt8);
	}
}
