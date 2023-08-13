#include "Tests.h"
#include "Misc.h"
#include "Control.h"

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
#include "TM_CommonCPP/ToLogStr.h"
#include "TM_CommonCPP/StdStringFromFormatString.h"
#include "TM_CommonCPP_NarrateOverloads.h"
#include "obse/Script.h"
// #include "obse/Hooks_DirectInput8Create.h"
#include <sstream>

#include "Globals.h"
#include "Settings.h"
#include "DebugCC.h"
#include "ExecuteCommand.h"



//### CommandTemplate
bool Cmd_CommandTemplate_Execute(COMMAND_ARGS)
{
	Logd(std::string(__func__) + "`Open");
	Logd(std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(CommandTemplate, "CommandTemplate command", 0, 0, nullptr)
//### Test1
bool Cmd_Test1_Execute(COMMAND_ARGS)
{
	Logd(std::string(__func__) + "`IsMenuMode():"+ TMC::ToLogStr(IsMenuMode()));
	*result = IsMenuMode();
	return true;
}
DEFINE_COMMAND_PLUGIN(Test1, "Test1 command", 0, 0, nullptr)
//### TestControlsFromString
bool Cmd_TestControlsFromString_Execute(COMMAND_ARGS)
{
	Logd(std::string(__func__) + "`Open");
	Logd("Controls:" + TMC::ToLogStr(Controls));
	std::string sControls = Controls.Stringize();
	Logd("sControls:" + sControls);
	auto cReturningControls = ControlCollection(sControls);
	Logd("cReturningControls:" + TMC::ToLogStr(cReturningControls));
	Logd(std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(TestControlsFromString, "TestControlsFromString command", 0, 0, nullptr)
//### TestControlToString
bool Cmd_TestControlToString_Execute(COMMAND_ARGS)
{
	Logd(std::string(__func__) + "`Open");
	std::string sControl = Controls.Items[0].ToString();
	Logd("sControl:" + sControl);
	std::vector<std::string> cStrings = TMC::Str::Split(sControl, ",");
	Logd("cStrings:" + TMC::ToLogStr(cStrings));
	Control vControl = Control(sControl);
	Logd("vControl:" + vControl.Narrate());
	Logd(std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(TestControlToString, "TestControlToString command", 0, 0, nullptr)
//### PrintControls
bool Cmd_PrintControls_Execute(COMMAND_ARGS)
{
	Logd("PrintControls`Controls:" + TMC::ToLogStr(Controls));
	return true;
}
DEFINE_COMMAND_PLUGIN(PrintControls, "PrintControls command", 0, 0, nullptr)
//### TestCeil
bool Cmd_TestCeil_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, TESObjectREFR * arg3, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
{
	// Logd(std::string(__func__) + "`Open");
	// *result = 0;
	// UInt8* fArgs = new UInt8[3 + sizeof(double)];
	// UInt16* fArgsNumArgs = (UInt16*)fArgs;
	// *fArgsNumArgs = 1;
	// fArgs[2] = 0x7A; // argument type double
	// double* fArgsVal = (double*)&fArgs[3];
	// *fArgsVal = 18.42;
	// UInt32 opOffsetPtr = 0;
	// const CommandInfo* ceil = g_commandTableIntfc->GetByName("Ceil");
	// ceil->execute(kParams_OneFloat, fArgs, thisObj, arg3, scriptObj, eventList, result, &opOffsetPtr);
	// delete[] fArgs;
	// Logd("TestCeil`opcode:" + TMC::ToLogStr(ceil->opcode) + " *result:" + TMC::ToLogStr(*result) + " result:" + TMC::ToLogStr(result));
	// Logd(std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(TestCeil, "TestCeil command", 0, 0, nullptr)
//### BasicRuntimeTests
bool Cmd_BasicRuntimeTests_Execute(COMMAND_ARGS)
{
	Logd(std::string(__func__) + "`Open");
	//*result = 0; //Do I need this?
	int iInt = 5;
	Logd("5:" + TMC::ToLogStr(iInt));
	UInt8 vUInt8 = 3;
	Logd("3:" + TMC::ToLogStr(vUInt8));
	std::set<UInt8> cSet;
	cSet.insert(65);
	cSet.insert(64);
	cSet.insert(63);
	Logd("Set:" + TMC::ToLogStr(cSet));
	Logd("ActualControls:" + TMC::ToLogStr(Controls));
	//static std::vector<Control> Controls_Fake;
	//Controls_Fake.push_back(Control(15,UInt32(4)));
	//for (Control &vControl : Controls_Fake)
	//{
	//	vControl.cModIndices_Disables.insert(222);
	//}
	//Logv("FakeControls:" + TMC::Narrate(Controls_Fake));
	Logd(std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(BasicRuntimeTests, "BasicRuntimeTests command", 0, 0, nullptr)
//### TestGetControlDirectly
bool Cmd_TestGetControlDirectly_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, TESObjectREFR * contObj, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
{
	Logd(std::string(__func__) + "`Open");
	double endResult;
	endResult = ExecuteCommand(GetControl_CmdInfo, 2, PASS_COMMAND_ARGS);
	// Report
	//Logv("TestGetControlDirectly`opcode:" + TMC::Narrate(GetControl->opcode) + " *result:" + TMC::Narrate(*result) + " result:" + TMC::Narrate(result));
	Logd("endResult:" + TMC::ToLogStr(endResult));
	Logd(std::string(__func__) + "`Open");
	return true;
}
DEFINE_COMMAND_PLUGIN(TestGetControlDirectly, "TestGetControlDirectly command", 0, 0, nullptr)
//### TestGetControlDirectly2
bool Cmd_TestGetControlDirectly2_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, TESObjectREFR * arg3, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
{
	Logd(std::string(__func__) + "`Open");
	double endResult;
	endResult = ExecuteCommand(GetControl_CmdInfo, 2);
	// Report
	//Logv("TestGetControlDirectly2`opcode:" + TMC::Narrate(GetControl->opcode) + " *result:" + TMC::Narrate(*result) + " result:" + TMC::Narrate(result));
	Logd("endResult:" + TMC::ToLogStr(endResult));
	Logd(std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(TestGetControlDirectly2, "TestGetControlDirectly2 command", 0, 0, nullptr)
//### TestGetControlCopyPasta
bool Cmd_TestGetControlCopyPasta_Execute(COMMAND_ARGS)
{
	Logd(std::string(__func__) + "`Open");
	*result = 0xFFFF;
	UInt32	keycode = 0;
	//ExtractArgs
	if (!ExtractArgs(paramInfo, arg1, opcodeOffsetPtr, thisObj, contObj, scriptObj, eventList, &keycode)) return true;
	//
	if (!InputControls) GetControlMap();
	*result = InputControls[keycode];
	Logd(std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(TestGetControlCopyPasta, "TestGetControlCopyPasta command", 0, 1, kParams_OneInt)
//### GenerateEnum
bool Cmd_GenerateEnum_Execute(COMMAND_ARGS)
{
	Logd(std::string(__func__) + "`Open");
	//Extract Args
	TESForm* rTemp = nullptr;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &rTemp)) {
		Logd(std::string(__func__) + "`Failed arg extraction");
		return false;
	}
	//Report
	Logd("rTemp:" + TMC::ToLogStr(rTemp->refID));
	//Return
	*result = rTemp->refID;
	Logd(std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(GenerateEnum, "GenerateEnum command", 0, 1, kParams_OneObjectRef)

