#include "Globals.h"
#include "Misc.h"

Cmd_Execute DisableKey_OriginalExecute = NULL; //Execute of replaced DisableKey command
Cmd_Execute EnableKey_OriginalExecute = NULL; //Execute of replaced EnableKey command
const CommandInfo* DisableKey_CmdInfo; //DisableKey command
const CommandInfo* GetControl; //GetControl command
std::vector<Control> Controls;

Script* pBlankScript = NULL;
ScriptEventList * pBlankScriptEventList = NULL;
bool bOblivionLoaded = false;
OBSESerializationInterface	* g_serialization = NULL;
PluginHandle				g_pluginHandle = kPluginHandle_Invalid;
