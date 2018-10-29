#pragma once
#include "TM_CommonCPP/Properties.h"
#include <string>
#include <vector>
#include <set>

class Control
{
private:
	int dxScancode_NonVanilla;
public:
	static const int VanillaControlID_EnumSize = 29;
	enum VanillaControlID
	{
		Forward,
		Back,
		Left,
		Right,
		Attack,
		Activate,
		Block,
		Cast,
		ReadyWeapon,
		Sneak,
		Run,
		AlwaysRun,
		AutoMove,
		Jump,
		TogglePOV,
		OpenMenu,
		Rest,
		QuickMenu,
		Quick1,
		Quick2,
		Quick3,
		Quick4,
		Quick5,
		Quick6,
		Quick7,
		Quick8,
		QuickSave,
		QuickLoad,
		Grab
	};
	static const int MenuModeType_EnumSize = 3;
	enum MenuModeType
	{
		GameMode,
		MenuMode,
		Both,
	};
	//---Constructors
	Control(UInt32 _ControlID, MenuModeType eMenuModeType, int _dxScancode_NonVanilla);
	Control(UInt32 _ControlID);
	Control(std::string sString);
	~Control(void);
	//---
	std::string ToString();
	std::string Narrate();
	const bool IsDisabled();
	void SetOutcome();
	void ResolveModIndices();
	int GetDXScancode();
	const bool IsPressed();
	const bool IsEngaged();
	//---Members
	std::set<UInt8> cModIndices_Disables;
	std::set<UInt8> cModIndices_UnreportedDisables;
	UInt32 ControlID;
	MenuModeType eMenuModeType; //Some controls should only be disabled during gamemode, etc.
	std::set<UInt32> cScriptRefs_ReceivedOnControlDown;
	bool bDeleteMe; // Ugly
};
