#pragma once
#include "Misc.h"
//#include "Globals.h"
#include "Settings.h"

extern IDebugLog		gLog; //Log
void FnDebug(std::string sString);
#define DebugCC(iLvl,sTxt) if (DebugThreshold >= iLvl) {FnDebug(sTxt);};
