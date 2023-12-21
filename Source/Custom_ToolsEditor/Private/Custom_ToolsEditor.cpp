#include "Custom_ToolsEditor.h"

#define LOCTEXT_NAMESPACE "FCustom_ToolsEditorModule"
#include "FExampleCommands.h"
#include "ToolMenu.h"
#include "ToolMenus.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Misc/MessageDialog.h"
#include "Widgets/Docking/SDockTab.h"

namespace Tools
{
	static const FName sTabName(TEXT("CustomTab"));
	static const FName sLevelEditorMenu(TEXT("LevelEditor.MainMenu.Window"));
}

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
				//FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("PopupMsg", "Hey world!"));
				FGlobalTabmanager::Get()->TryInvokeTab(Tools::sTabName);
			})
		)
		);
	Section.AddEntry(MenuEntry);


	
	//UToolMenu* Menu = UToolMenus::Get()->ExtendMenu(TEXT("LevelEditor.MainMenu.Window"));
	//{
	//	FToolMenuSection& Section = Menu->FindOrAddSection(TEXT("WindowLayout"));
	//	{
	//		FToolMenuEntry& Entry = Section.AddEntry( FToolMenuEntry::InitMenuEntry(FExampleCommands::Get().CommandInfo_ShowMsg));
	//		Entry.SetCommandList(ShowMsg);
	//		
	//		//Section.AddEntry()
	//	}
	//}
}

 TSharedRef<SDockTab>  FCustom_ToolsEditorModule::OnTabSpawn(const FSpawnTabArgs& SpawnTabArgs)
 {
	 return SNew(SDockTab).TabRole(ETabRole::NomadTab);
}

void FCustom_ToolsEditorModule::StartupModule()
{
	//Setup
//FExampleCommands::Register();
//ShowMsg = MakeShareable(new FUICommandList);
//
//ShowMsg->MapAction(FExampleCommands::Get().CommandInfo_ShowMsg,
//FExecuteAction::CreateStatic(FExampleCommands::Command_ShowMsg)
//);
//
//
////Callback
// UToolMenus::RegisterStartupCallback(
// 	FSimpleMulticastDelegate::FDelegate::CreateRaw(this,&FCustom_ToolsEditorModule::RegisterMenus));
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(Tools::sTabName,FOnSpawnTab::CreateRaw(this,&FCustom_ToolsEditorModule::OnTabSpawn)).SetDisplayName(LOCTEXT("TabName","Ameisin tab"));
	RegisterMenus();
}

void FCustom_ToolsEditorModule::ShutdownModule()
{
    
}



#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FCustom_ToolsEditorModule, Custom_ToolsEditor)