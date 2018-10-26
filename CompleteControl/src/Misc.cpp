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



//### StringizeControls
std::string StringizeControls(std::vector<Control> cControls)
{
	std::stringstream ss;
	for (auto vControl : cControls)
	{
		ss << "`" << vControl.ToString();
	}
	return ss.str();
}
//### ControlsFromString
std::vector<Control> ControlsFromString(std::string sBigString)
{
	std::vector<Control> cReturningControls;
	for (auto s : TMC::SplitString(sBigString, "`"))
	{
		if (s.empty()) { continue; }
		cReturningControls.push_back(Control(s));
	}
	return cReturningControls;
}
//### SafeConsolePrint
void SafeConsolePrint(std::string sString)
{
	if (!bOblivionLoaded) // Trying to print to console without Oblivion loaded causes CTD.
	{
		//do nothing
	}
	else if (sString.length() < 1000) //1000 might not be the exact limit. Exceeding limit causes CTD.
	{
		Console_Print(sString.c_str());
	}
	else
	{
		Console_Print(sString.substr(0, 1000).c_str());
		Console_Print("<MessageTooLarge>");
	}
}
//### ExecuteCommand
double ExecuteCommand(Cmd_Execute vCmdExecute, double vArg, COMMAND_ARGS)
{
	// Overload specific vars
	int iDataTypeCode = 0x7A; //double
	//
	double result2 = 0;
	UInt8* pData = new UInt8[3 + sizeof(double)];
	UInt16* vNumArgs = (UInt16*)pData;
	*vNumArgs = 1;
	pData[2] = iDataTypeCode;
	double* fArgsVal = (double*)&pData[3];
	*fArgsVal = vArg;
	UInt32 opOffsetPtr2 = 0;
	vCmdExecute(kParams_OneInt, pData, thisObj, arg3, scriptObj, eventList, &result2, &opOffsetPtr2);
	delete[] pData;
	return result2;
}
double ExecuteCommand(Cmd_Execute vCmdExecute, double vArg)
{
	ParamInfo * paramInfo = NULL;
	void * arg1 = 0;
	TESObjectREFR * thisObj = NULL;
	UInt32 arg3 = 0;
	if (!pBlankScript)
	{
		DebugCC(6, "ExecuteCommand`pBlankScript INIT");
		pBlankScript = (Script*)CreateFormInstance(13);
		pBlankScriptEventList = (*pBlankScript).CreateEventList();
	}
	Script * scriptObj = pBlankScript;
	ScriptEventList * eventList = pBlankScriptEventList;
	double * result = 0;
	UInt32 * opcodeOffsetPtr = 0;
	return ExecuteCommand(vCmdExecute, vArg, PASS_COMMAND_ARGS);
}
double ExecuteCommand(const CommandInfo* vCmd, double vArg, COMMAND_ARGS)
{
	return ExecuteCommand(vCmd->execute, vArg, PASS_COMMAND_ARGS);
}
double ExecuteCommand(const CommandInfo* vCmd, double vArg)
{
	return ExecuteCommand(vCmd->execute, vArg);
}
//### InitializeControls
std::vector<Control> InitializeControls()
{
	DebugCC(5, "InitializeControls`Open");
	std::vector<Control> cControls;
	for (int i = 0; i < Control::VanillaControlID_Count; ++i)
	{
		cControls.push_back(Control(ExecuteCommand(GetControl, i), i));
	}
	DebugCC(6, "InitializeControls`cControls:" + TMC::Narrate(cControls)); //verbose
	DebugCC(5, "InitializeControls`Close");
	return cControls;
}

