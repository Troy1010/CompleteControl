#include "TM_CommonCPP_NarrateOverloads.h"
#include "TM_CommonCPP/Narrate.h"
#include "Control.h"
#include <sstream>

namespace TMC
{
	std::string Narrate(std::vector<Control> Controls)
	{
		std::ostringstream vReturning;
		TMC::Narrator::iIndent++;
		vReturning << "Collection(Count:" << Narrate((int)Controls.size()) << ")..";
		for (Control vControl : Controls)
		{
			vReturning << "\n" << TMC::Narrator::Indent() << vControl.Narrate();
		}
		TMC::Narrator::iIndent--;
		return vReturning.str();
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
}
