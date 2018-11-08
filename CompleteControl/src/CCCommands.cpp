#include "CCCommands.h"

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
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &dxScancode)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	auto pControl = GetControlByScancode(dxScancode);
	//-Unregister disable. SetOutcome.
	pControl->cModIndices_Disables.erase((UInt8)(scriptObj->refID >> 24)); //iModIndex:(UInt8)(scriptObj->refID >> 24)
	pControl->SetOutcome();
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(EnableKey_Replacing, "Unregisters disable of this mod. Enables key if there are no disables registered.", 0, 1, kParams_OneInt)
//### DisableControl
void DisableControl_Helper(UInt32 iControlID, UInt8 iModIndex)
{
	Control* pControl;
	if (!(pControl = GetControlByID(iControlID))) { DebugCC(5, std::string(__func__) + "`Received unregistered ControlID:" + TMC::Narrate(iControlID)); return; }
	pControl->cModIndices_Disables.insert(iModIndex);
	pControl->SetOutcome();
}
bool Cmd_DisableControl_Replacing_Execute(COMMAND_ARGS)
{
	DebugCC(5, std::string(__func__) + "`Open");
	UInt32	iControlID = 0;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &iControlID)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	DisableControl_Helper(iControlID, (UInt8)(scriptObj->refID >> 24)); //iModIndex:(UInt8)(scriptObj->refID >> 24)
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(DisableControl_Replacing, "Disables a key and registers a disable", 0, 1, kParams_OneInt)
bool Cmd_DisableControl_ByRef_Execute(COMMAND_ARGS)
{
	DebugCC(5, std::string(__func__) + "`Open");
	TESForm* vControlID_Form = 0;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &vControlID_Form)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	DisableControl_Helper(vControlID_Form->refID, (UInt8)(scriptObj->refID >> 24)); //iModIndex:(UInt8)(scriptObj->refID >> 24)
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(DisableControl_ByRef, "Disables a key and registers a disable", 0, 1, kParams_OneRef)
bool Cmd_DisableControls_Execute(COMMAND_ARGS)
{
	DebugCC(5, std::string(__func__) + "`Open");
	UInt32	iControlID1 = -1; //Perhaps I could use an array here? Honestly though, ExtractArgs is spooky enough.
	UInt32	iControlID2 = -1;
	UInt32	iControlID3 = -1;
	UInt32	iControlID4 = -1;
	UInt32	iControlID5 = -1;
	UInt32	iControlID6 = -1;
	UInt32	iControlID7 = -1;
	UInt32	iControlID8 = -1;
	UInt32	iControlID9 = -1;
	UInt32	iControlID10 = -1;
	UInt32	iControlID11 = -1;
	UInt32	iControlID12 = -1;
	UInt32	iControlID13 = -1;
	UInt32	iControlID14 = -1;
	UInt32	iControlID15 = -1;
	UInt32	iControlID16 = -1;
	UInt32	iControlID17 = -1;
	UInt32	iControlID18 = -1;
	UInt32	iControlID19 = -1;
	UInt32	iControlID20 = -1;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &iControlID1, &iControlID2, &iControlID3, &iControlID4, &iControlID5, &iControlID6, &iControlID7, &iControlID8, &iControlID9, &iControlID10, &iControlID11, &iControlID12, &iControlID13, &iControlID14, &iControlID15, &iControlID16, &iControlID17, &iControlID18, &iControlID19, &iControlID20)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	UInt8 iModIndex = (UInt8)(scriptObj->refID >> 24);
	DisableControl_Helper(iControlID1, iModIndex);
	DisableControl_Helper(iControlID2, iModIndex);
	DisableControl_Helper(iControlID3, iModIndex);
	DisableControl_Helper(iControlID4, iModIndex);
	DisableControl_Helper(iControlID5, iModIndex);
	DisableControl_Helper(iControlID6, iModIndex);
	DisableControl_Helper(iControlID7, iModIndex);
	DisableControl_Helper(iControlID8, iModIndex);
	DisableControl_Helper(iControlID9, iModIndex);
	DisableControl_Helper(iControlID10, iModIndex);
	DisableControl_Helper(iControlID11, iModIndex);
	DisableControl_Helper(iControlID12, iModIndex);
	DisableControl_Helper(iControlID13, iModIndex);
	DisableControl_Helper(iControlID14, iModIndex);
	DisableControl_Helper(iControlID15, iModIndex);
	DisableControl_Helper(iControlID16, iModIndex);
	DisableControl_Helper(iControlID17, iModIndex);
	DisableControl_Helper(iControlID18, iModIndex);
	DisableControl_Helper(iControlID19, iModIndex);
	DisableControl_Helper(iControlID20, iModIndex);
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(DisableControls, "Disables a key and registers a disable", 0, 20, kParams_20OptionalInts)
bool Cmd_DisableControls_ByRef_Execute(COMMAND_ARGS)
{
	DebugCC(5, std::string(__func__) + "`Open");
	TESForm* vControlID_Form1 = 0;
	TESForm* vControlID_Form2 = 0;
	TESForm* vControlID_Form3 = 0;
	TESForm* vControlID_Form4 = 0;
	TESForm* vControlID_Form5 = 0;
	TESForm* vControlID_Form6 = 0;
	TESForm* vControlID_Form7 = 0;
	TESForm* vControlID_Form8 = 0;
	TESForm* vControlID_Form9 = 0;
	TESForm* vControlID_Form10 = 0;
	TESForm* vControlID_Form11 = 0;
	TESForm* vControlID_Form12 = 0;
	TESForm* vControlID_Form13 = 0;
	TESForm* vControlID_Form14 = 0;
	TESForm* vControlID_Form15 = 0;
	TESForm* vControlID_Form16 = 0;
	TESForm* vControlID_Form17 = 0;
	TESForm* vControlID_Form18 = 0;
	TESForm* vControlID_Form19 = 0;
	TESForm* vControlID_Form20 = 0;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &vControlID_Form1, &vControlID_Form2, &vControlID_Form3, &vControlID_Form4, &vControlID_Form5, &vControlID_Form6, &vControlID_Form7, &vControlID_Form8, &vControlID_Form9, &vControlID_Form10, &vControlID_Form11, &vControlID_Form12, &vControlID_Form13, &vControlID_Form14, &vControlID_Form15, &vControlID_Form16, &vControlID_Form17, &vControlID_Form18, &vControlID_Form19, &vControlID_Form20)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	UInt8 iModIndex = (UInt8)(scriptObj->refID >> 24);
	DisableControl_Helper(vControlID_Form1->refID, iModIndex);
	DisableControl_Helper(vControlID_Form2->refID, iModIndex);
	DisableControl_Helper(vControlID_Form3->refID, iModIndex);
	DisableControl_Helper(vControlID_Form4->refID, iModIndex);
	DisableControl_Helper(vControlID_Form5->refID, iModIndex);
	DisableControl_Helper(vControlID_Form6->refID, iModIndex);
	DisableControl_Helper(vControlID_Form7->refID, iModIndex);
	DisableControl_Helper(vControlID_Form8->refID, iModIndex);
	DisableControl_Helper(vControlID_Form9->refID, iModIndex);
	DisableControl_Helper(vControlID_Form10->refID, iModIndex);
	DisableControl_Helper(vControlID_Form11->refID, iModIndex);
	DisableControl_Helper(vControlID_Form12->refID, iModIndex);
	DisableControl_Helper(vControlID_Form13->refID, iModIndex);
	DisableControl_Helper(vControlID_Form14->refID, iModIndex);
	DisableControl_Helper(vControlID_Form15->refID, iModIndex);
	DisableControl_Helper(vControlID_Form16->refID, iModIndex);
	DisableControl_Helper(vControlID_Form17->refID, iModIndex);
	DisableControl_Helper(vControlID_Form18->refID, iModIndex);
	DisableControl_Helper(vControlID_Form19->refID, iModIndex);
	DisableControl_Helper(vControlID_Form20->refID, iModIndex);
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(DisableControls, "Disables a key and registers a disable", 0, 20, kParams_20OptionalInts)
//### EnableControl
void EnableControl_Helper(UInt32 iControlID, UInt8 iModIndex)
{
	Control* pControl;
	if (!(pControl = GetControlByID(iControlID))) { DebugCC(5, std::string(__func__) + "`Received unregistered ControlID:" + TMC::Narrate(iControlID)); return; }
	pControl->cModIndices_Disables.erase(iModIndex);
	pControl->SetOutcome();
}
bool Cmd_EnableControl_Replacing_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, UInt32 arg3, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
{
	DebugCC(5, std::string(__func__) + "`Open");
	UInt32	iControlID = 0;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &iControlID)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	EnableControl_Helper(iControlID, (UInt8)(scriptObj->refID >> 24)); //iModIndex:(UInt8)(scriptObj->refID >> 24)
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(EnableControl_Replacing, "Unregisters disable of this mod. Enables key if there are no disables registered.", 0, 1, kParams_OneInt)
bool Cmd_EnableControl_ByRef_Execute(COMMAND_ARGS)
{
	DebugCC(5, std::string(__func__) + "`Open");
	TESForm* vControlID_Form = 0;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &vControlID_Form)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	EnableControl_Helper(vControlID_Form->refID, (UInt8)(scriptObj->refID >> 24)); //iModIndex:(UInt8)(scriptObj->refID >> 24)
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(EnableControl_ByRef, "Unregisters disable of this mod. Enables key if there are no disables registered.", 0, 1, kParams_OneRef)
//### RegisterControl
bool Cmd_RegisterControl_Execute(COMMAND_ARGS)
{
	DebugCC(5, std::string(__func__) + "`Open");
	TESForm* vControlID_Form = 0;
	int dxScancode = 0;
	int iMenuModeType = -1;
	auto eMenuModeType = Control::MenuModeType(0);
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &vControlID_Form, &dxScancode, &iMenuModeType)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	//-Defaults for optional params
	if (!dxScancode) { dxScancode = 0xFF; } //0xFF = unassigned
	if (iMenuModeType != -1) { eMenuModeType = Control::MenuModeType(iMenuModeType); }
	//-
	Controls.push_back(Control(vControlID_Form->refID, eMenuModeType, dxScancode));
	DebugCC(6, std::string(__func__) + "`Controls:" + TMC::Narrate(Controls));
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(RegisterControl, "Registers a non-vanilla Control. Uses any ref as its ID.", 0, 3, kParams_RegisterControl)
//### IsDisabled
bool IsDisabled_Helper(UInt32 iControlID)
{
	Control* pControl;
	if (!(pControl = GetControlByID(iControlID))) { DebugCC(5, std::string(__func__) + "`Received unregistered ControlID:" + TMC::Narrate(iControlID)); return false; }
	return pControl->IsDisabled();
}
bool Cmd_IsDisabled_Execute(COMMAND_ARGS)
{
	DebugCC(5, std::string(__func__) + "`Open");
	UInt32	vControlID = 0;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &vControlID)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	*result = IsDisabled_Helper(vControlID);
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(IsDisabled, "True if the key -should- be disabled.", 0, 1, kParams_OneInt)
bool Cmd_IsDisabled_ByRef_Execute(COMMAND_ARGS)
{
	DebugCC(5, std::string(__func__) + "`Open");
	TESForm* vControlID_Form = 0;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &vControlID_Form)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	*result = IsDisabled_Helper(vControlID_Form->refID);
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(IsDisabled_ByRef, "True if the key -should- be disabled.", 0, 1, kParams_OneRef)
//### GetKey
double GetKey_Helper(UInt32 iControlID)
{
	Control* pControl;
	if (!(pControl = GetControlByID(iControlID))) { DebugCC(5, std::string(__func__) + "`Received unregistered ControlID:" + TMC::Narrate(iControlID)); return 0xFF; }
	return pControl->GetDXScancode();
}
bool Cmd_GetKey_Execute(COMMAND_ARGS)
{
	DebugCC(5, std::string(__func__) + "`Open");
	UInt32	iControlID = 0;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &iControlID)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	*result = GetKey_Helper(iControlID);
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(GetKey, "GetKey command", 0, 1, kParams_OneInt)
bool Cmd_GetKey_ByRef_Execute(COMMAND_ARGS)
{
	DebugCC(5, std::string(__func__) + "`Open");
	TESForm* vControlID_Form = 0;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &vControlID_Form)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	*result = GetKey_Helper(vControlID_Form->refID);
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(GetKey_ByRef, "GetKey_ByRef command", 0, 1, kParams_OneRef)
//### UnreportedDisable
void UnreportedDisable_Helper(UInt32 iControlID, UInt8 iModIndex)
{
	auto pControl = GetControlByID(iControlID);
	pControl->cModIndices_UnreportedDisables.insert(iModIndex);
	pControl->SetOutcome();
}
bool Cmd_UnreportedDisable_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, UInt32 arg3, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
{
	DebugCC(5, std::string(__func__) + "`Open");
	UInt32	iControlID = 0;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &iControlID)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	UnreportedDisable_Helper(iControlID, (UInt8)(scriptObj->refID >> 24)); //iModIndex:(UInt8)(scriptObj->refID >> 24)
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(UnreportedDisable, "Disables a key and registers a disable", 0, 1, kParams_OneInt)
bool Cmd_UnreportedDisable_ByRef_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, UInt32 arg3, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
{
	DebugCC(5, std::string(__func__) + "`Open");
	TESForm* vControlID_Form = 0;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &vControlID_Form)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	UnreportedDisable_Helper(vControlID_Form->refID, (UInt8)(scriptObj->refID >> 24)); //iModIndex:(UInt8)(scriptObj->refID >> 24)
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(UnreportedDisable_ByRef, "Disables a key and registers a disable", 0, 1, kParams_OneRef)
//### UnreportedEnable
void UnreportedEnable_Helper(UInt32 iControlID, UInt8 iModIndex)
{
	auto pControl = GetControlByID(iControlID);
	pControl->cModIndices_UnreportedDisables.erase(iModIndex);
	pControl->SetOutcome();
}
bool Cmd_UnreportedEnable_Execute(COMMAND_ARGS)
{
	DebugCC(5, std::string(__func__) + "`Open");
	UInt32	iControlID = 0;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &iControlID)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	UnreportedEnable_Helper(iControlID, (UInt8)(scriptObj->refID >> 24)); //iModIndex:(UInt8)(scriptObj->refID >> 24)
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(UnreportedEnable, "Unregisters disable of this mod. Enables key if there are no disables registered.", 0, 1, kParams_OneInt)
bool Cmd_UnreportedEnable_ByRef_Execute(COMMAND_ARGS)
{
	DebugCC(5, std::string(__func__) + "`Open");
	TESForm* vControlID_Form = 0;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &vControlID_Form)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	UnreportedEnable_Helper(vControlID_Form->refID, (UInt8)(scriptObj->refID >> 24)); //iModIndex:(UInt8)(scriptObj->refID >> 24)
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(UnreportedEnable_ByRef, "Unregisters disable of this mod. Enables key if there are no disables registered.", 0, 1, kParams_OneRef)
//### IsEngaged
bool IsEngaged_Helper(UInt32 iControlID)
{
	Control* pControl;
	if (!(pControl = GetControlByID(iControlID))) { DebugCC(5, std::string(__func__) + "`Received unregistered ControlID:" + TMC::Narrate(iControlID)); return false; }
	return pControl->IsEngaged();
}
bool Cmd_IsEngaged_Execute(COMMAND_ARGS)
{
	DebugCC(5, std::string(__func__) + "`Open. scriptObjRefID:"+ TMC::Narrate(scriptObj->refID));
	UInt32	iControlID = -1;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &iControlID)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	*result = IsEngaged_Helper(iControlID);
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(IsEngaged, "Is the control pressed and not disabled?", 0, 1, kParams_OneInt)
bool Cmd_IsEngaged_ByRef_Execute(COMMAND_ARGS)
{
	DebugCC(5, std::string(__func__) + "`Open");
	TESForm* vControlID_Form = 0;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &vControlID_Form)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	*result = IsEngaged_Helper(vControlID_Form->refID);
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(IsEngaged_ByRef, "Is the control pressed and not disabled?", 0, 1, kParams_OneRef)
//### OnControlDown2
bool OnControlDown2_Helper(UInt32 iControlID, UInt32 iRefID)
{
	Control* pControl;
	if (!(pControl = GetControlByID(iControlID))) { DebugCC(5, std::string(__func__) + "`Received unregistered ControlID:" + TMC::Narrate(iControlID)); return false; }
	if (pControl->IsPressed())
	{
		if (!Contains(pControl->cScriptRefs_ReceivedOnControlDown, iRefID))
		{
			pControl->cScriptRefs_ReceivedOnControlDown.insert(iRefID);
			return true;
		}
	}
	else if (Contains(pControl->cScriptRefs_ReceivedOnControlDown, iRefID))
	{
		pControl->cScriptRefs_ReceivedOnControlDown.erase(iRefID);
	}
	return false;
}
bool Cmd_OnControlDown2_Execute(COMMAND_ARGS)
{
	DebugCC(8, std::string(__func__) + "`Open");
	UInt32	iControlID = 0;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &iControlID)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	*result = OnControlDown2_Helper(iControlID, scriptObj->refID);
	DebugCC(8, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(OnControlDown2, "OnControlDown2 command", 0, 1, kParams_OneInt)
bool Cmd_OnControlDown2_ByRef_Execute(COMMAND_ARGS)
{
	DebugCC(8, std::string(__func__) + "`Open");
	TESForm* vControlID_Form = 0;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &vControlID_Form)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	*result = OnControlDown2_Helper(vControlID_Form->refID, scriptObj->refID);
	DebugCC(8, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(OnControlDown2_ByRef, "OnControlDown2 command", 0, 1, kParams_OneRef)
