#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"




class FCustom_ToolsEditorModule : public IModuleInterface
{
    FDelegateHandle RegisterMenu_DelegateHandle;
    TSharedPtr<FUICommandList> ShowMsg;
public:
    TSharedRef<SDockTab> OnTabSpawn(const FSpawnTabArgs& SpawnTabArgs);
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    
    static FCustom_ToolsEditorModule& Get()
    {
        return FModuleManager::LoadModuleChecked<FCustom_ToolsEditorModule>(TEXT("Custom_ToolsEditorModule"));
    }
private:
    
    void RegisterMenus();
};
