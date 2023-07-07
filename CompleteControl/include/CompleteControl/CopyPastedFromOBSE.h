#pragma once

#include "obse/PluginAPI.h"
#include "obse/CommandTable.h"
#if OBLIVION
#include "obse/GameAPI.h"
#else
#include "obse_editor/EditorAPI.h"
#endif
#include "obse/ParamInfos.h"
#include <vector>
#include <set>
#include "Control.h"
#include <string>
#include "TM_CommonCPP/ToLogStr.h"
#include "TM_CommonCPP/StdStringFromFormatString.h"
#include "TM_CommonCPP_NarrateOverloads.h"
#include "obse/Script.h"
// #include "obse/Hooks_DirectInput8Create.h"
#include <sstream>


// Copy-pasted from obse's Control_Input
#define CONTROLSMAPPED 29
//Roundabout way of getting a pointer to the array containing control map
//Not sure what CtrlMapBaseAddr points to (no RTTI) so use brute pointer arithmetic
#if OBLIVION_VERSION == OBLIVION_VERSION_1_1
static const UInt32* CtrlMapBaseAddr = (UInt32*)0x00AEAAB8;
static const UInt32	 CtrlMapOffset = 0x00000020;
static const UInt32  CtrlMapOffset2 = 0x00001B7E;
#elif OBLIVION_VERSION == OBLIVION_VERSION_1_2
static const UInt32* CtrlMapBaseAddr = (UInt32*)0x00B33398;
static const UInt32	 CtrlMapOffset = 0x00000020;
static const UInt32  CtrlMapOffset2 = 0x00001B7E;
#elif OBLIVION_VERSION == OBLIVION_VERSION_1_2_416
extern const UInt32* CtrlMapBaseAddr;
extern const UInt32	 CtrlMapOffset;
extern const UInt32  CtrlMapOffset2;
#endif
extern UInt8*  InputControls;
extern UInt8*  AltInputControls;
void GetControlMap();
bool IsKeycodeValid(UInt32 id);
