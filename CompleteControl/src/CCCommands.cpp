#include "CCCommands.h"

//### CommandTemplate
bool Cmd_CommandTemplate_Execute(COMMAND_ARGS)
{
	DebugCC(5, "CommandTemplate`Open");
	DebugCC(5, "CommandTemplate`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(CommandTemplate, "CommandTemplate command", 0, 0, NULL)
//### DisableKey_Replacing
bool Cmd_DisableKey_Replacing_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, UInt32 arg3, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
{
	DebugCC(5, "Cmd_DisableKey_Replacing_Execute`Open");
	UInt32	dxScancode = 0;
	UInt8	iModIndex = 0;
	//---Register in Controls
	//-Get dxScancode
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &dxScancode)) {
		DebugCC(5, "Cmd_DisableKey_Replacing_Execute`Failed arg extraction");
		return false;
	}
	//-Get iModIndex
	iModIndex = (UInt8)(scriptObj->refID >> 24);
	//-Register iModIndex in vControl.cModIndices
	for (Control &vControl : Controls)
	{
		if (vControl.dxScancode == dxScancode)
		{
			vControl.cModIndices_Disables.insert(iModIndex);
			break;
		}
	}
	//---DisableKey
	//-Execute Original DisableKey
	DisableKey_OriginalExecute(PASS_COMMAND_ARGS);
	DebugCC(5, "Cmd_DisableKey_Replacing_Execute`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(DisableKey_Replacing, "Disables a key and registers a disable", 0, 1, kParams_OneInt)
//### EnableKey_Replacing
bool Cmd_EnableKey_Replacing_Execute(ParamInfo * paramInfo, void * arg1, TESObjectREFR * thisObj, UInt32 arg3, Script * scriptObj, ScriptEventList * eventList, double * result, UInt32 * opcodeOffsetPtr)
{
	DebugCC(5, "Cmd_EnableKey_Replacing_Execute`Open");
	UInt32	dxScancode = 0;
	UInt8	iModIndex = 0;
	//---Register in Controls
	//-Get dxScancode
	if (!ExtractArgsEx(paramInfo, arg1, opcodeOffsetPtr, scriptObj, eventList, &dxScancode)) {
		DebugCC(5, "Cmd_EnableKey_Replacing_Execute`Failed arg extraction");
		return false;
	}
	//-Get iModIndex
	iModIndex = (UInt8)(scriptObj->refID >> 24);
	//-Unregister disable. Determine bDoEnableKey by checking if any disables are registered for our dxScancode
	bool bDoEnableKey = true;
	for (Control &vControl : Controls)
	{
		if (vControl.dxScancode == dxScancode)
		{
			vControl.cModIndices_Disables.erase(iModIndex);
			if (!(vControl.cModIndices_Disables.empty()))
			{
				bDoEnableKey = false;
			}
			break;
		}
	}
	//---Execute original EnableKey
	if (bDoEnableKey) {
		DebugCC(5, "Cmd_EnableKey_Replacing_Execute`EnablingKey");
		EnableKey_OriginalExecute(PASS_COMMAND_ARGS);
	}
	else
	{
		DebugCC(5, "Cmd_EnableKey_Replacing_Execute`Neglecting to enable key.");
	}
	DebugCC(5, "Cmd_EnableKey_Replacing_Execute`Close");
	return true;
}
DEFINE_COMMAND_PLUGIN(EnableKey_Replacing, "Unregisters disable of this mod. Enables key if there are no disables registered.", 0, 1, kParams_OneInt)