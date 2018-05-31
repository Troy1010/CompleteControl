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
#include "obse/Hooks_DirectInput8Create.h"


using namespace std;
IDebugLog		gLog("CompleteControl.log");

OBSECommandTableInterface	* g_commandTableIntfc = NULL;

static vector<Control> Controls;


std::string strcat2(const char * sTemp1, const char * sTemp2)
{
	std::string sReturning(sTemp1);
	sReturning = sReturning.append(sTemp2);
	return sReturning;
}

const char * strcat3(const char * sTemp1, const char * sTemp2)
{
	char sReturning[100];
	strcpy(sReturning, sTemp1);
	strcat(sReturning, sTemp2);
	return sReturning;
}


// DisableW
bool Cmd_DisableW_Execute(COMMAND_ARGS)
{
	*result = 0;
	//UInt32	keycode = 17; //W:17
	//DI_data.DisallowStates[keycode] = 0x00;
	const CommandInfo * kCommandInfo_DisableKey =  g_commandTableIntfc->GetByName("DisableKey");
	//kCommandInfo_DisableKey->execute(paramInfo, arg1, thisObj, arg3, scriptObj, eventList, result, opcodeOffsetPtr);

	kCommandInfo_DisableKey->execute(PASS_COMMAND_ARGS);

	Console_Print(strcat3("kCommandInfo_DisableKey->shortName: ", kCommandInfo_DisableKey->shortName));
	Console_Print("Cmd_DisableW_Execute`Close");

	// Report Success
	return true;
}
DEFINE_COMMAND_PLUGIN(DisableW, "Disables the W key", 0, 1, kParams_OneInt)


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
	obse->RegisterCommand(&kCommandInfo_DisableW);
	//obse->RegisterCommand(&kCommandInfo_TestExtractFormatString);
	if(!obse->isEditor)
	{
		// get an OBSEScriptInterface to use for argument extraction
		g_scriptInterface = (OBSEScriptInterface*)obse->QueryInterface(kInterface_Script);
	}

	g_commandTableIntfc = (OBSECommandTableInterface*)obse->QueryInterface(kInterface_CommandTable);

	return true;
}
};
