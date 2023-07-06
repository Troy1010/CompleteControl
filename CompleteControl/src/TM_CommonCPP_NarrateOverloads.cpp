#include "TM_CommonCPP_NarrateOverloads.h"
#include <sstream>

#include "TM_CommonCPP/ToLogStr.h"

namespace TMC
{
	std::string ToLogStr(Control Control)
	{
		return "TODO";
		// return Control.Narrate();
	}
	std::string ToLogStr(std::vector<Control> Controls)
	{
		return "TODO";
		// return LogStrFactory::ToLogStr_Collection(Controls);
	}
	std::string ToLogStr(std::map<UInt32, Control> Controls)
	{
		return "TODO";
		// return LogStrFactory::ToLogStr_Collection(Controls);
	}
	std::string ToLogStr(std::set<UInt8> cSet)
	{
		return "TODO";
		// return LogStrFactory::ToLogStr_Collection(cSet);
	}
	std::string ToLogStr(UInt8 vUInt8)
	{
		return "TODO";
		// return std::to_string(vUInt8);
	}
	std::string ToLogStr(UInt32 vUInt32)
	{
		return "TODO";
		// return std::to_string(vUInt32);
	}
	std::string ToLogStr(std::pair<UInt32, Control> vPair)
	{
		return "TODO";
		// return ToLogStr(vPair.first) + ":" + ToLogStr(vPair.second);
	}
	std::string ToLogStr(ControlCollection Controls)
	{
		return "TODO";
		// return LogStrFactory::ToLogStr_Collection(Controls.Items);
	}
}
