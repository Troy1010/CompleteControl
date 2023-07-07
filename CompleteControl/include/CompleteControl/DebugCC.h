#pragma once
#include "Misc.h"
//#include "Globals.h"
#include "Settings.h"
#include "TM_CommonCPP/String.h"

extern IDebugLog		gLog; //Log
void FnDebug(std::string sString);
#define DebugCC(iLvl,sTxt) if (DebugThreshold >= (iLvl)) {FnDebug(sTxt);};
#define logz(sTxt) if (DebugThreshold >= -1) {FnDebug(sTxt);};
#define Logi(sTxt) if (DebugThreshold >= 1) {FnDebug(sTxt);};
#define Loge(sTxt) if (DebugThreshold >= 1) {FnDebug("!ERR!`" + std::string(sTxt));};
#define Logw(sTxt) if (DebugThreshold >= 1) {FnDebug("!WARN!`" + std::string(sTxt));};
#define Logd(sTxt) if (DebugThreshold >= 2) {FnDebug(sTxt);};
#define Logv(sTxt) if (DebugThreshold >= 6) {FnDebug(sTxt);};