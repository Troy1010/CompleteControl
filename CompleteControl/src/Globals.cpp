#include "Globals.h"
#include "Misc.h"

#if OBLIVION
OBSECommandTableInterface * g_commandTableIntfc = NULL; // assigned in OBSEPlugin_Load
OBSEScriptInterface * g_scriptIntfc = NULL; //For command argument extraction
#define ExtractArgsEx(...) g_scriptIntfc->ExtractArgsEx(__VA_ARGS__)
#define ExtractFormatStringArgs(...) g_scriptIntfc->ExtractFormatStringArgs(__VA_ARGS__)
#endif

Cmd_Execute DisableKey_OriginalExecute = NULL; //Execute of replaced DisableKey command
Cmd_Execute EnableKey_OriginalExecute = NULL; //Execute of replaced EnableKey command
const CommandInfo* GetControl_CmdInfo; //GetControl command
const CommandInfo* ResolveModIndex_CmdInfo;
const CommandInfo* IsKeyPressed3_CmdInfo;
std::vector<Control> Controls;

Script* pBlankScript = NULL;
ScriptEventList * pBlankScriptEventList = NULL;
OBSESerializationInterface	* g_serialization = NULL;
PluginHandle				g_pluginHandle = kPluginHandle_Invalid;

bool bOblivionLoaded = false; //Because there is no OblivionLoaded event, this variable is set true later than it should be. (without a helper mod)
bool bMenuMode = true; //As far as I know, I must use a helper mod for this.
