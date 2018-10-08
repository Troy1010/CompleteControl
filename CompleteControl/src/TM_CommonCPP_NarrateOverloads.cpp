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
		TM_CommonCPP::Narrator::iIndent++;
		for each (Control vControl in Controls)
		{
			vReturning << "\n" << TM_CommonCPP::Narrator::Indent() << i << ":" <<vControl.Narrate() ;
			i++;
		}
		TM_CommonCPP::Narrator::iIndent--;
		return "NarrateControls(Count:" + Narrate(i) + ").." + vReturning.str();
	}
	std::string Narrate(std::set<UInt8> cSet)
	{
		return Narrator::Narrate_Collection(cSet);
	}
	std::string Narrate(UInt8 vUInt8)
	{
		return std::to_string(vUInt8);
	}
}
