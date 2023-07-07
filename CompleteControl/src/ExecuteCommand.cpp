#include "ExecuteCommand.h"

double ExecuteCommand(Cmd_Execute vCmdExecute, double vArg, COMMAND_ARGS)
{
	int iDataTypeCode = 0x7A; //doubleTypeID, apparently
	double result2 = 0;
	UInt8* pData = new UInt8[3 + sizeof(double)];
	UInt16* vNumArgs = (UInt16*)pData;
	*vNumArgs = 1;
	pData[2] = iDataTypeCode;
	double* fArgsVal = (double*)&pData[3];
	*fArgsVal = vArg;
	UInt32 opOffsetPtr2 = 0;
	vCmdExecute(kParams_OneInt, pData, thisObj, contObj, scriptObj, eventList, &result2, &opOffsetPtr2);
	delete[] pData;
	return result2;
}
double ExecuteCommand(Cmd_Execute vCmdExecute, double vArg)
{
	ParamInfo * paramInfo = NULL;
	void * arg1 = 0;
	TESObjectREFR * thisObj = NULL;
	TESObjectREFR * contObj = NULL;
	if (!pBlankScript)
	{
		Logv(std::string(__func__) + "`pBlankScript INIT");
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
