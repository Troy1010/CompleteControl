#include "DebugCC.h"

IDebugLog		gLog(DebugLogName); //Log

void FnDebug(std::string sString)
{
	SafeConsolePrint(sString);
	_MESSAGE(sString.c_str());
}
