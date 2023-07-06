#include "CCCommands.h"
#include "TM_CommonCPP/ToLogStr.h"

//### DisableKey2
bool Cmd_DisableKey2_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, TESObjectREFR* contObj, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
{
	DebugCC(5, std::string(__func__) + "`Open");
	UInt32	iDXScancode = 0;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &iDXScancode)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	auto pControl = Controls.FindByScancode(iDXScancode);
	if (!pControl) { DebugCC(5, std::string(__func__) + "`Could not retrieve pControl with iDXScancode:" + TMC::ToLogStr(iDXScancode)); return true; }
	//-Register iModIndex in vControl.cModIndices. SetOutcome.
	pControl->cModIndices_Disables.insert((UInt8)(scriptObj->refID >> 24)); //iModIndex:(UInt8)(scriptObj->refID >> 24)
	pControl->SetOutcome();
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(DisableKey2, "Disables a key and registers a disable", 0, 1, kParams_OneInt)
//### EnableKey2
bool Cmd_EnableKey2_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, TESObjectREFR * contObj, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
{
	DebugCC(5, std::string(__func__) + "`Open");
	UInt32	iDXScancode = 0;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &iDXScancode)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	auto pControl = Controls.FindByScancode(iDXScancode);
	if (!pControl) { DebugCC(5, std::string(__func__) + "`Could not retrieve pControl with iDXScancode:" + TMC::ToLogStr(iDXScancode)); return true; }
	//-Unregister disable. SetOutcome.
	pControl->cModIndices_Disables.erase((UInt8)(scriptObj->refID >> 24)); //iModIndex:(UInt8)(scriptObj->refID >> 24)
	pControl->SetOutcome();
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(EnableKey2, "Unregisters disable of this mod. Enables key if there are no disables registered.", 0, 1, kParams_OneInt)
//### DisableControl
void DisableControl_Helper(UInt32 iControlID, UInt8 iModIndex)
{
	auto pControl = Controls.FindByID(iControlID);
	if (!pControl) { DebugCC(7, std::string(__func__) + "`Received unregistered ControlID:" + TMC::ToLogStr(iControlID)); return; }
	pControl->cModIndices_Disables.insert(iModIndex);
	pControl->SetOutcome();
}
bool Cmd_DisableControl2_Execute(COMMAND_ARGS)
{
	DebugCC(5, std::string(__func__) + "`Open");
	UInt32	iControlID = 0;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &iControlID)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	DisableControl_Helper(iControlID, (UInt8)(scriptObj->refID >> 24)); //iModIndex:(UInt8)(scriptObj->refID >> 24)
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(DisableControl2, "Disables a key and registers a disable", 0, 1, kParams_OneInt)
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
	UInt32 arrID = 0;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &arrID)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	UInt8 iModIndex = (UInt8)(scriptObj->refID >> 24);
	OBSEArray* arr = g_arrayIntfc->LookupArrayByID(arrID);
	if (arr) {
		UInt32 size = g_arrayIntfc->GetArraySize(arr);
		if (size != -1) {
			OBSEElement* elems = new OBSEElement[size];
			OBSEElement* keys = new OBSEElement[size];

			if (g_arrayIntfc->GetElements(arr, elems, keys)) {
				OBSEArray* newArr = g_arrayIntfc->CreateArray(NULL, 0, scriptObj);
				for (UInt32 i = 0; i < size; i++) {
					if (elems[i].GetType() == OBSEElement::kType_Numeric) {
						DebugCC(6, std::string(__func__) + "`Disabling:"+ TMC::ToLogStr(elems[i].Number()));
						DisableControl_Helper(elems[i].Number(), iModIndex);
					}
					else if (elems[i].GetType() == OBSEElement::kType_Form) {
						DebugCC(6, std::string(__func__) + "`Disabling:" + TMC::ToLogStr((UInt32)elems[i].Form()->refID));
						DisableControl_Helper(elems[i].Form()->refID, iModIndex);
					}
					else {
						DebugCC(5, std::string(__func__) + "`Received invalid type for ControlID");
					}
				}
			}

			delete[] elems;
			delete[] keys;
		}
	}
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(DisableControls, "Takes an array of ControlIDs or ControlRefs and disables each one.", 0, 1, kParams_Array)
//### EnableControl
void EnableControl_Helper(UInt32 iControlID, UInt8 iModIndex)
{
	auto pControl = Controls.FindByID(iControlID);
	if (!pControl) { DebugCC(7, std::string(__func__) + "`Received unregistered ControlID:" + TMC::ToLogStr(iControlID)); return; }
	pControl->cModIndices_Disables.erase(iModIndex);
	pControl->SetOutcome();
}
bool Cmd_EnableControl2_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, TESObjectREFR * contObj, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
{
	DebugCC(5, std::string(__func__) + "`Open");
	UInt32	iControlID = 0;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &iControlID)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	EnableControl_Helper(iControlID, (UInt8)(scriptObj->refID >> 24)); //iModIndex:(UInt8)(scriptObj->refID >> 24)
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(EnableControl2, "Unregisters disable of this mod. Enables key if there are no disables registered.", 0, 1, kParams_OneInt)
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
bool Cmd_EnableControls_Execute(COMMAND_ARGS)
{
	DebugCC(5, std::string(__func__) + "`Open");
	UInt32 arrID = 0;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &arrID)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	UInt8 iModIndex = (UInt8)(scriptObj->refID >> 24);
	OBSEArray* arr = g_arrayIntfc->LookupArrayByID(arrID);
	if (arr) {
		UInt32 size = g_arrayIntfc->GetArraySize(arr);
		if (size != -1) {
			OBSEElement* elems = new OBSEElement[size];
			OBSEElement* keys = new OBSEElement[size];

			if (g_arrayIntfc->GetElements(arr, elems, keys)) {
				OBSEArray* newArr = g_arrayIntfc->CreateArray(NULL, 0, scriptObj);
				for (UInt32 i = 0; i < size; i++) {
					if (elems[i].GetType() == OBSEElement::kType_Numeric) {
						DebugCC(6, std::string(__func__) + "`Enabling:" + TMC::ToLogStr(elems[i].Number()));
						EnableControl_Helper(elems[i].Number(), iModIndex);
					}
					else if (elems[i].GetType() == OBSEElement::kType_Form) {
						DebugCC(6, std::string(__func__) + "`Enabling:" + TMC::ToLogStr((UInt32)elems[i].Form()->refID));
						EnableControl_Helper(elems[i].Form()->refID, iModIndex);
					}
					else {
						DebugCC(5, std::string(__func__) + "`Received invalid type for ControlID");
					}
				}
			}

			delete[] elems;
			delete[] keys;
		}
	}
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(EnableControls, "Takes an array of ControlIDs or ControlRefs and enables each one.", 0, 1, kParams_Array)
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
	auto vControl = Control(vControlID_Form->refID, eMenuModeType, dxScancode);
	Controls.Items.insert({ vControl.ControlID ,vControl });
	DebugCC(6, std::string(__func__) + "`Controls:" + TMC::ToLogStr(Controls));
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(RegisterControl, "Registers a non-vanilla Control. Uses any ref as its ID. Recommended to call at InitGameMode0.", 0, 3, kParams_RegisterControl)
//### IsDisabled
bool IsDisabled_Helper(UInt32 iControlID)
{
	auto pControl = Controls.FindByID(iControlID);
	if (!pControl) { DebugCC(7, std::string(__func__) + "`Received unregistered ControlID:" + TMC::ToLogStr(iControlID)); return false; }
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
	auto pControl = Controls.FindByID(iControlID);
	if (!pControl) { DebugCC(7, std::string(__func__) + "`Received unregistered ControlID:" + TMC::ToLogStr(iControlID)); return 0xFF; }
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
	auto pControl = Controls.FindByID(iControlID);
	if (!pControl) { DebugCC(7, std::string(__func__) + "`Received unregistered ControlID:" + TMC::ToLogStr(iControlID)); return; }
	pControl->cModIndices_UnreportedDisables.insert(iModIndex);
	pControl->SetOutcome();
}
bool Cmd_UnreportedDisable_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, TESObjectREFR * contObj, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
{
	DebugCC(5, std::string(__func__) + "`Open");
	UInt32	iControlID = 0;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &iControlID)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	UnreportedDisable_Helper(iControlID, (UInt8)(scriptObj->refID >> 24)); //iModIndex:(UInt8)(scriptObj->refID >> 24)
	DebugCC(5, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(UnreportedDisable, "Disables a key and registers a disable", 0, 1, kParams_OneInt)
bool Cmd_UnreportedDisable_ByRef_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, TESObjectREFR * contObj, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
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
	auto pControl = Controls.FindByID(iControlID);
	if (!pControl) { DebugCC(7, std::string(__func__) + "`Received unregistered ControlID:" + TMC::ToLogStr(iControlID)); return; }
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
	auto pControl = Controls.FindByID(iControlID);
	if (!pControl) { DebugCC(7, std::string(__func__) + "`Received unregistered ControlID:" + TMC::ToLogStr(iControlID)); return false; }
	return pControl->IsEngaged();
}
bool Cmd_IsEngaged_Execute(COMMAND_ARGS)
{
	DebugCC(6, std::string(__func__) + "`Open. scriptObjRefID:"+ TMC::ToLogStr(scriptObj->refID));
	UInt32	iControlID = -1;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &iControlID)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	*result = IsEngaged_Helper(iControlID);
	DebugCC(6, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(IsEngaged, "Is the control pressed and not disabled?", 0, 1, kParams_OneInt)
bool Cmd_IsEngaged_ByRef_Execute(COMMAND_ARGS)
{
	DebugCC(6, std::string(__func__) + "`Open");
	TESForm* vControlID_Form = 0;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &vControlID_Form)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	*result = IsEngaged_Helper(vControlID_Form->refID);
	DebugCC(6, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(IsEngaged_ByRef, "Is the control pressed and not disabled?", 0, 1, kParams_OneRef)
//### IsPressed
bool IsPressed_Helper(UInt32 iControlID)
{
	auto pControl = Controls.FindByID(iControlID);
	if (!pControl) { DebugCC(7, std::string(__func__) + "`Received unregistered ControlID:" + TMC::ToLogStr(iControlID)); return false; }
	return pControl->IsPressed();
}
bool Cmd_IsPressed_Execute(COMMAND_ARGS)
{
	DebugCC(6, std::string(__func__) + "`Open. scriptObjRefID:" + TMC::ToLogStr(scriptObj->refID));
	UInt32	iControlID = -1;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &iControlID)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	*result = IsPressed_Helper(iControlID);
	DebugCC(6, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(IsPressed, "Is the control pressed regardless of if it's disabled?", 0, 1, kParams_OneInt)
bool Cmd_IsPressed_ByRef_Execute(COMMAND_ARGS)
{
	DebugCC(6, std::string(__func__) + "`Open");
	TESForm* vControlID_Form = 0;
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &vControlID_Form)) { DebugCC(5, std::string(__func__) + "`Failed arg extraction"); return false; }
	*result = IsPressed_Helper(vControlID_Form->refID);
	DebugCC(6, std::string(__func__) + "`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(IsPressed_ByRef, "Is the control pressed regardless of if it's disabled?", 0, 1, kParams_OneRef)
//### OnControlDown2
bool OnControlDown2_Helper(UInt32 iControlID, UInt32 iRefID)
{
	auto pControl = Controls.FindByID(iControlID);
	if (!pControl) { DebugCC(7, std::string(__func__) + "`Received unregistered ControlID:" + TMC::ToLogStr(iControlID)); return false; }
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
