#include "Custom_ToolsEditor.h"

#define LOCTEXT_NAMESPACE "FCustom_ToolsEditorModule"
#include "ToolMenu.h"
#include "ToolMenus.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Misc/MessageDialog.h"

void FCustom_ToolsEditorModule::RegisterMenus()
{
	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu(TEXT("LevelEditor.MainMenu.Window"));
	FToolMenuSection& Section = Menu->FindOrAddSection(TEXT("WindowLayout"));

	FToolMenuEntry MenuEntry = FToolMenuEntry::InitMenuEntry(
	TEXT("Dsp_Msg"),
	LOCTEXT("DisplayMsg", "Display Message"),
	LOCTEXT("DisplayMsg_Tooltip", "A window will popip!"),
	FSlateIcon(),
	FUIAction(
			FExecuteAction::CreateLambda([]()
			{
				FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("PopupMsg", "Hey world!"));
			})
		)
		);
	Section.AddEntry(MenuEntry);
}

void FCustom_ToolsEditorModule::StartupModule()
{
   //UToolMenus::RegisterStartupCallback();
}

void FCustom_ToolsEditorModule::ShutdownModule()
{
    
}



#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FCustom_ToolsEditorModule, Custom_ToolsEditor)