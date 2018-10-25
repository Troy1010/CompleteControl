#include "DebugCC.h"

IDebugLog		gLog(DebugLogName); //Log

//### FnDebug
void FnDebug(std::string sString)
{
	SafeConsolePrint(sString);
	_MESSAGE(sString.c_str());
}
