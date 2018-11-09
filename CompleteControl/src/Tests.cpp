#include "Tests.h"
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
#include "ExecuteCommand.h"



//### CommandTemplate
bool Cmd_CommandTemplate_Execute(COMMAND_ARGS)
{
	DebugCC(5, std::string(__func__) + "`Open");
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(CommandTemplate, "CommandTemplate command", 0, 0, NULL)
//### Test1
bool Cmd_Test1_Execute(COMMAND_ARGS)
{
	DebugCC(5, std::string(__func__) + "`IsMenuMode():"+ TMC::Narrate(IsMenuMode()));
	*result = IsMenuMode();
	return true;
}
DEFINE_COMMAND_PLUGIN(Test1, "Test1 command", 0, 0, NULL)
//### TestControlsFromString
bool Cmd_TestControlsFromString_Execute(COMMAND_ARGS)
{
	DebugCC(4, std::string(__func__) + "`Open");
	DebugCC(4, "Controls:" + TMC::Narrate(Controls));
	std::string sControls = StringizeControls(Controls);
	DebugCC(4, "sControls:" + sControls);
	std::map<int, Control> cReturningControls = ControlsFromString(sControls);
	DebugCC(4, "cReturningControls:" + TMC::Narrate(cReturningControls));
	DebugCC(4, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(TestControlsFromString, "TestControlsFromString command", 0, 0, NULL)
//### TestControlToString
bool Cmd_TestControlToString_Execute(COMMAND_ARGS)
{
	DebugCC(4, std::string(__func__) + "`Open");
	std::string sControl = Controls[0].ToString();
	DebugCC(4, "sControl:" + sControl);
	std::vector<std::string> cStrings = TMC::SplitString(sControl, ",");
	DebugCC(4, "cStrings:" + TMC::Narrate(cStrings));
	Control vControl = Control(sControl);
	DebugCC(4, "vControl:" + vControl.Narrate());
	DebugCC(4, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(TestControlToString, "TestControlToString command", 0, 0, NULL)
//### PrintControls
bool Cmd_PrintControls_Execute(COMMAND_ARGS)
{
	DebugCC(4, "PrintControls`Controls:" + TMC::Narrate(Controls));
	return true;
}
DEFINE_COMMAND_PLUGIN(PrintControls, "PrintControls command", 0, 0, NULL)
//### TestCeil
bool Cmd_TestCeil_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, UInt32 arg3, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
{
	DebugCC(4, std::string(__func__) + "`Open");
	*result = 0;
	UInt8* fArgs = new UInt8[3 + sizeof(double)];
	UInt16* fArgsNumArgs = (UInt16*)fArgs;
	*fArgsNumArgs = 1;
	fArgs[2] = 0x7A; // argument type double
	double* fArgsVal = (double*)&fArgs[3];
	*fArgsVal = 18.42;
	UInt32 opOffsetPtr = 0;
	const CommandInfo* ceil = g_commandTableIntfc->GetByName("Ceil");
	ceil->execute(kParams_OneFloat, fArgs, thisObj, arg3, scriptObj, eventList, result, &opOffsetPtr);
	delete[] fArgs;
	DebugCC(4, "TestCeil`opcode:" + TMC::Narrate(ceil->opcode) + " *result:" + TMC::Narrate(*result) + " result:" + TMC::Narrate(result));
	DebugCC(4, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(TestCeil, "TestCeil command", 0, 0, NULL)
//### BasicRuntimeTests
bool Cmd_BasicRuntimeTests_Execute(COMMAND_ARGS)
{
	DebugCC(4, std::string(__func__) + "`Open");
	//*result = 0; //Do I need this?
	int iInt = 5;
	DebugCC(4, "5:" + TMC::Narrate(iInt));
	UInt8 vUInt8 = 3;
	DebugCC(4, "3:" + TMC::Narrate(vUInt8));
	std::set<UInt8> cSet;
	cSet.insert(65);
	cSet.insert(64);
	cSet.insert(63);
	DebugCC(4, "Set:" + TMC::Narrate(cSet));
	DebugCC(4, "ActualControls:" + TMC::Narrate(Controls));
	//static std::vector<Control> Controls_Fake;
	//Controls_Fake.push_back(Control(15,UInt32(4)));
	//for (Control &vControl : Controls_Fake)
	//{
	//	vControl.cModIndices_Disables.insert(222);
	//}
	//DebugCC(5,"FakeControls:" + TMC::Narrate(Controls_Fake));
	DebugCC(4, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(BasicRuntimeTests, "BasicRuntimeTests command", 0, 0, NULL)
//### TestGetControlDirectly
bool Cmd_TestGetControlDirectly_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, UInt32 arg3, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
{
	DebugCC(4, std::string(__func__) + "`Open");
	double endResult;
	endResult = ExecuteCommand(GetControl_CmdInfo, 2, PASS_COMMAND_ARGS);
	// Report
	//DebugCC(5,"TestGetControlDirectly`opcode:" + TMC::Narrate(GetControl->opcode) + " *result:" + TMC::Narrate(*result) + " result:" + TMC::Narrate(result));
	DebugCC(4, "endResult:" + TMC::Narrate(endResult));
	DebugCC(4, std::string(__func__) + "`Open");
	return true;
}
DEFINE_COMMAND_PLUGIN(TestGetControlDirectly, "TestGetControlDirectly command", 0, 0, NULL)
//### TestGetControlDirectly2
bool Cmd_TestGetControlDirectly2_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, UInt32 arg3, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
{
	DebugCC(4, std::string(__func__) + "`Open");
	double endResult;
	endResult = ExecuteCommand(GetControl_CmdInfo, 2);
	// Report
	//DebugCC(5,"TestGetControlDirectly2`opcode:" + TMC::Narrate(GetControl->opcode) + " *result:" + TMC::Narrate(*result) + " result:" + TMC::Narrate(result));
	DebugCC(5, "endResult:" + TMC::Narrate(endResult));
	DebugCC(4, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(TestGetControlDirectly2, "TestGetControlDirectly2 command", 0, 0, NULL)
//### TestGetControlCopyPasta
bool Cmd_TestGetControlCopyPasta_Execute(COMMAND_ARGS)
{
	DebugCC(4, std::string(__func__) + "`Open");
	*result = 0xFFFF;
	UInt32	keycode = 0;
	//ExtractArgs
	if (!ExtractArgs(paramInfo, arg1, opcodeOffsetPtr, thisObj, arg3, scriptObj, eventList, &keycode)) return true;
	//
	if (!InputControls) GetControlMap();
	*result = InputControls[keycode];
	DebugCC(4, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(TestGetControlCopyPasta, "TestGetControlCopyPasta command", 0, 1, kParams_OneInt)
//### TestDisableKeyCopyPasta
bool Cmd_TestDisableKeyCopyPasta_Execute(COMMAND_ARGS)
{
	DebugCC(4, std::string(__func__) + "`Open");
	*result = 0;
	UInt32	keycode = 0;
	//
	if (!ExtractArgs(paramInfo, arg1, opcodeOffsetPtr, thisObj, arg3, scriptObj, eventList, &keycode)) return true;
	if (keycode % 256 == 255 && keycode < 2048) keycode = 255 + (keycode + 1) / 256;
	if (IsKeycodeValid(keycode)) DI_data.DisallowStates[keycode] = 0x00;
	DebugCC(4, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(TestDisableKeyCopyPasta, "TestDisableKeyCopyPasta command", 0, 0, NULL)
//### GenerateEnum
bool Cmd_GenerateEnum_Execute(COMMAND_ARGS)
{
	DebugCC(4, std::string(__func__) + "`Open");
	//Extract Args
	TESForm* rTemp = NULL;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &rTemp)) {
		DebugCC(5, std::string(__func__) + "`Failed arg extraction");
		return false;
	}
	//Report
	DebugCC(5, "rTemp:" + TMC::Narrate(rTemp->refID));
	//Return
	*result = rTemp->refID;
	DebugCC(4, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(GenerateEnum, "GenerateEnum command", 0, 1, kParams_OneObjectRef)

