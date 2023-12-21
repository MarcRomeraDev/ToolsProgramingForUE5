#include "FExampleCommands.h"

#define LOCTEXT_NAMESPACE "FCustom_ToolsEditorModule"

void FExampleCommands::RegisterCommands()
{
	UI_COMMAND(CommandInfo_ShowMsg, "ShowHelloWindow","Show the hello window", EUserInterfaceActionType::Button,FInputChord());
}

#undef LOCTEXT_NAMESPACE