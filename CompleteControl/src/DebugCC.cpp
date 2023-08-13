#include "DebugCC.h"

// IDebugLog		gLog(DebugLogName); //Log
//
void FnDebug(std::string sString)
{
	SafeConsolePrint(sString);
	_MESSAGE(TMC::Str::Replace(sString,"%","").c_str());
}
