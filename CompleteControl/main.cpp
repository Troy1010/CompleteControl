#include "obse/PluginAPI.h"
#include "obse/CommandTable.h"
#include "obse_common/SafeWrite.cpp"
#if OBLIVION
#include "obse/GameAPI.h"
OBSEScriptInterface * g_scriptInterface = NULL; // assigned in OBSEPlugin_Load
#define ExtractArgsEx(...) g_scriptInterface->ExtractArgsEx(__VA_ARGS__)
#define ExtractFormatStringArgs(...) g_scriptInterface->ExtractFormatStringArgs(__VA_ARGS__)
#else
#include "obse_editor/EditorAPI.h"
#endif
#include "obse/ParamInfos.h"
#include <vector>
#include "Control.h"

using namespace std;
IDebugLog		gLog("CompleteControl.log");

static vector<Control> Controls;
// Helper Function
bool RegisterVanillaControls()
{
	//Controls.push_back(Control(Control::GenerateFormID(),17,0,0,GameMode)); //*Grab dxScancodes from Oblivion.ini
	Controls.push_back(Control(Control::GenerateFormID(),30,0,0,Control::MenuModeType::GameMode));
	Controls.push_back(Control(Control::GenerateFormID(),31,0,0,Control::MenuModeType::GameMode));
	Controls.push_back(Control(Control::GenerateFormID(),32,0,0,Control::MenuModeType::GameMode));
	Control* Control1 = new Control();
	Control1->~Control();
	return true;
}


extern "C" {
bool OBSEPlugin_Query(const OBSEInterface * obse, PluginInfo * info)
{
	_MESSAGE("query");
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "CompleteControl";
	info->version = 1;
	_MESSAGE("OBSE version: %08X",obse->oblivionVersion); //BUG: For some reason, obse->obseVersion gives 15 when it should give 21
	// version checks
	if(!obse->isEditor)
	{
#if OBLIVION
		if(obse->oblivionVersion != OBLIVION_VERSION)
		{
			_ERROR("incorrect Oblivion version (got %08X need %08X)", obse->oblivionVersion, OBLIVION_VERSION);
			return false;
		}
#endif	
	}
	return true;
}
bool OBSEPlugin_Load(const OBSEInterface * obse)
{
	_MESSAGE("load");
	obse->SetOpcodeBase(0x28B0);
	//obse->RegisterCommand(&kCommandInfo_TestExtractFormatString);
	if(!obse->isEditor)
	{
		// get an OBSEScriptInterface to use for argument extraction
		g_scriptInterface = (OBSEScriptInterface*)obse->QueryInterface(kInterface_Script);
	}

	//vector<Control> Controls; //Controls [ID:ControlObject][dxScancode,DisableCount,OverrideCount,MenuModeType,OnControlDownStringmap]
	//Controls.push_back(Control(1,1,1,1,Control::MenuModeType::GameMode)); //? I don't have to write new?
	//Controls[0] = ;
	
	//Controls.push_back(Control(1,1,1,1,Control::MenuModeType::GameMode));
	RegisterVanillaControls();
	for (Control _Control : Controls)
	{
		//for each (var 
		//_Control;
	};


	return true;
}
};
