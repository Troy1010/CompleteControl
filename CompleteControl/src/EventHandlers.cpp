#include "Misc.h"
#include "Control.h"
#include "TM_CommonCPP/Misc.h"

#include "obse/PluginAPI.h"
#include "obse/CommandTable.h"
#if OBLIVION
#include "obse/GameAPI.h"
#else
#include "obse_editor/EditorAPI.h"
#endif
#include "obse/ParamInfos.h"
#include <vector>
#include <set>
#include "Control.h"
#include <string>
#include "TM_CommonCPP/Misc.h"
#include "TM_CommonCPP/Narrate.h"
#include "TM_CommonCPP/StdStringFromFormatString.h"
#include "TM_CommonCPP_NarrateOverloads.h"
#include "obse/Script.h"
#include "obse/Hooks_DirectInput8Create.h"
#include <sstream>

#include "Globals.h"
#include "Settings.h"
#include "DebugCC.h"



void Handler_Save(void * reserved)
{
	//-Write Control
	std::string sControls = StringizeControls(Controls);
	g_serialization->WriteRecord('CTRL', 0, sControls.c_str(), sControls.size());
}

void Handler_Load(void * reserved)
{
	UInt32	type, version, length;
	char* buf;
	while (g_serialization->GetNextRecordInfo(&type, &version, &length))
	{
		DebugCC(5, TMC::StdStringFromFormatString("record %08X (%.4s) %08X %08X", type, &type, version, length));
		switch (type)
		{
		case 'CTRL':
			buf = new char[length + 1]; //c strings require a null at the end.
			g_serialization->ReadRecordData(buf, length);
			buf[length] = 0;
			DebugCC(5, "buf:" + TMC::Narrate(buf));
			Controls = ControlsFromString(std::string(buf));
			DebugCC(5, "Controls:" + TMC::Narrate(Controls));
			delete buf;
			break;
		default:
			DebugCC(5, TMC::StdStringFromFormatString("Unknown chunk type %08X", type));
		}
	}
	if (Controls.empty())
	{
		Controls = InitializeControls();
	}
	//---Refresh Disables
}

void Handler_Preload(void * reserved)
{
	DebugCC(4, "Preload Callback start");
	Handler_Load(reserved);
	DebugCC(4, "Preload Callback finished");
}

void Handler_NewGame(void * reserved)
{
	Controls = InitializeControls();
}

