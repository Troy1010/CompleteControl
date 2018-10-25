#include "CopyPastedFromOBSE.h"


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
static const UInt32* CtrlMapBaseAddr = (UInt32*)0x00B33398;
static const UInt32	 CtrlMapOffset = 0x00000020;
static const UInt32  CtrlMapOffset2 = 0x00001B7E;
#endif
UInt8*  InputControls = 0;
UInt8*  AltInputControls = 0;
void GetControlMap()
{
	UInt32 addr = *CtrlMapBaseAddr + CtrlMapOffset;
	addr = *(UInt32*)addr + CtrlMapOffset2;
	InputControls = (UInt8*)addr;
	AltInputControls = InputControls + CONTROLSMAPPED;
}
bool IsKeycodeValid(UInt32 id) { return id < kMaxMacros - 2; }
