#include "ControlCollection.h"
#include "DebugCC.h"
#include "TM_CommonCPP/Misc.h"

std::pair<UInt32, Control> ControlCollection::KeyValuePair(Control& vControl)
{
	return std::pair<UInt32, Control>(vControl.ControlID, vControl);
}

void ControlCollection::RegisterVanillaControls()
{
	DebugCC(5, std::string(__func__) + "`Open");
	for (UInt32 i = 0; i < Control::VanillaControlID_EnumSize; ++i)
	{
		auto vControl = Control(i);
		if (i == 17)
		{
			vControl.eMenuModeType = Control::MenuModeType::Both;
		}
		Items.insert(KeyValuePair(vControl));
	}
	DebugCC(6, std::string(__func__) + "`cControls:" + TMC::Narrate(Items));
	DebugCC(5, std::string(__func__) + "`Close");
}

ControlCollection::ControlCollection()
{
}
ControlCollection::ControlCollection(const std::string& sControls)
{
	for (auto s : TMC::Str::Split(sControls, "`"))
	{
		if (s.empty()) { continue; }
		Items.insert(KeyValuePair(Control(s)));
	}
}

std::string ControlCollection::Stringize()
{
	std::stringstream ss;
	for (auto vKeyAndValue : Items)
	{
		ss << "`" << vKeyAndValue.second.ToString();
	}
	return ss.str();
}

void ControlCollection::EnableAll()
{
	for (auto& vControl : Items)
	{
		vControl.second.Enable();
	}
}
void ControlCollection::SetOutcomes()
{
	for (auto& vControl : Items)
	{
		vControl.second.SetOutcome();
	}
}

Control* ControlCollection::FindByScancode(int iDXScancode)
{
	for (auto&[_, vControl] : Items)
	{
		if (vControl.GetDXScancode() == iDXScancode)
		{
			return &vControl;
		}
	}
	return NULL;
}

Control* ControlCollection::FindByID(UInt32 iControlID)
{
	try {
		return &Items.at(iControlID);
	}
	catch (const std::out_of_range& e) {
		DebugCC(5, std::string(__func__) + "`Received invalid iControlID:" + TMC::Narrate(iControlID));
		return NULL;
	}
}
