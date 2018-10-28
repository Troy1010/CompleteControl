#include "CCCommands.h"

//### CommandTemplate
bool Cmd_CommandTemplate_Execute(COMMAND_ARGS)
{
	DebugCC(5, std::string(__func__) + "`Open");
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(CommandTemplate, "CommandTemplate command", 0, 0, NULL)
//### DisableKey_Replacing
bool Cmd_DisableKey_Replacing_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, UInt32 arg3, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
{
	DebugCC(5, std::string(__func__) + "`Open");
	UInt32	dxScancode = 0;
	UInt8	iModIndex = 0;
	//---Register in Controls
	//-Get dxScancode
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &dxScancode)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	//-Get iModIndex
	iModIndex = (UInt8)(scriptObj->refID >> 24);
	//-Register iModIndex in vControl.cModIndices. SetOutcome.
	auto vControl = GetControlByScancode(dxScancode);
	vControl->cModIndices_Disables.insert(iModIndex);
	vControl->SetOutcome();
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(DisableKey_Replacing, "Disables a key and registers a disable", 0, 1, kParams_OneInt)
//### EnableKey_Replacing
bool Cmd_EnableKey_Replacing_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, UInt32 arg3, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
{
	DebugCC(5, std::string(__func__) + "`Open");
	UInt32	dxScancode = 0;
	UInt8	iModIndex = 0;
	//---Register in Controls
	//-Get dxScancode
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &dxScancode)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	//-Get iModIndex
	iModIndex = (UInt8)(scriptObj->refID >> 24);
	//-Unregister disable. SetOutcome.
	auto pControl = GetControlByScancode(dxScancode);
	pControl->cModIndices_Disables.erase(iModIndex);
	pControl->SetOutcome();
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(EnableKey_Replacing, "Unregisters disable of this mod. Enables key if there are no disables registered.", 0, 1, kParams_OneInt)
//### DisableControl_Replacing
bool Cmd_DisableControl_Replacing_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, UInt32 arg3, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
{
	DebugCC(5, std::string(__func__) + "`Open");
	UInt32	vControlID = 0;
	UInt8	iModIndex = 0;
	//---Register in Controls
	//-Get vControlID
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &vControlID)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	//-Get iModIndex
	iModIndex = (UInt8)(scriptObj->refID >> 24);
	//-Register iModIndex in vControl.cModIndices. SetOutcome.
	auto pControl = GetControlByID(vControlID);
	pControl->cModIndices_Disables.insert(iModIndex);
	pControl->SetOutcome();
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(DisableControl_Replacing, "Disables a key and registers a disable", 0, 1, kParams_OneInt)
//### EnableControl_Replacing
bool Cmd_EnableControl_Replacing_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, UInt32 arg3, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
{
	DebugCC(5, std::string(__func__) + "`Open");
	UInt32	vControlID = 0;
	UInt8	iModIndex = 0;
	//---Register in Controls
	//-Get vControlID
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &vControlID)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	//-Get iModIndex
	iModIndex = (UInt8)(scriptObj->refID >> 24);
	//-Unregister disable. SetOutcome.
	auto vControl = GetControlByID(vControlID);
	vControl->cModIndices_Disables.erase(iModIndex);
	vControl->SetOutcome();
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(EnableControl_Replacing, "Unregisters disable of this mod. Enables key if there are no disables registered.", 0, 1, kParams_OneInt)
//### RegisterControl
bool Cmd_RegisterControl_Execute(COMMAND_ARGS)
{
	DebugCC(5, std::string(__func__) + "`Open");
	UInt32	vControlID = 0;
	int dxScancode = 0;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &vControlID, &dxScancode)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	Controls.push_back(Control(vControlID, dxScancode));
	DebugCC(4, std::string(__func__) + "`Controls:" + TMC::Narrate(Controls));
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(RegisterControl, "Registers a non-vanilla Control.", 0, 2, kParams_TwoInts)
