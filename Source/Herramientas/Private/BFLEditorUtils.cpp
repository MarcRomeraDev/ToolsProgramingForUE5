// Fill out your copyright notice in the Description page of Project Settings.


#include "BFLEditorUtils.h"
#include "ConditionLibrary.h"
#include "ObjectTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "DSP/PassiveFilter.h"
#include "Engine/Blueprint.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Misc/MessageDialog.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Misc/FileHelper.h"
#if WITH_EDITOR
void UBFLEditorUtils::GetCDOFromBlueprintAsset(const UObject* InBlueprintAsset, UObject*& OutCDO)
{
	// if(InBlueprintAsset && InBlueprintAsset->IsA(UBlueprint::StaticClass()))
	if (const UBlueprint* Blueprint = Cast<UBlueprint>(InBlueprintAsset))
	{
		OutCDO = Blueprint->GeneratedClass->GetDefaultObject();
	}
}

// auto SomeFunction() -> bool {}

void UBFLEditorUtils::ListTagAndValues(const FAssetData& InAssetData)
{
	FString Path;
	UBFLEditorUtils::GetExportsPath(Path);
}

bool UBFLEditorUtils::IsPowerOfTwo(int32 InValue)
{
	return FMath::IsPowerOfTwo(InValue);
}

void UBFLEditorUtils::GetExportsPath(FString& OutPath)
{
	OutPath = FPaths::ProjectSavedDir() / TEXT("Exports");
}

bool UBFLEditorUtils::GetAllAssetsOfClass(const UClass* InClass,
                                          TArray<FAssetData>& OutAssetData)
{
	if (!InClass)
	{
		FNotificationInfo NotificationInfo{FText::FromString(TEXT("Class is not set..."))};
		FSlateNotificationManager& NotificationManager = FSlateNotificationManager::Get();
		NotificationManager.AddNotification(NotificationInfo);

		return false;
	}

	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(
		"AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	// TArray<FAssetData> AssetData;

	FARFilter Filter;
	Filter.ClassPaths.Add(InClass->GetClassPathName());
	Filter.PackagePaths.Add(TEXT("/Game"));
	Filter.bRecursivePaths = true;

	AssetRegistry.GetAssets(Filter, OutAssetData);

	if (OutAssetData.IsEmpty())
	{
		const FNotificationInfo NotificationInfo{FText::FromString(TEXT("No he encontrado ningún asset"))};
		FSlateNotificationManager& NotificationManager = FSlateNotificationManager::Get();
		NotificationManager.AddNotification(NotificationInfo);
		return false;
	}

	const FNotificationInfo NotificationInfo{
		FText::FromString(
			FString::Printf(TEXT("He encontrado %d assets de tipo %s"), OutAssetData.Num(), *InClass->GetName()))
	};
	FSlateNotificationManager& NotificationManager = FSlateNotificationManager::Get();
	NotificationManager.AddNotification(NotificationInfo);

	return true;
}



void UBFLEditorUtils::ListAssetReferences()
{
	// Create a filter to get all assets in the project
	FARFilter Filter;
	Filter.PackagePaths.Add(TEXT("/Game"));
	Filter.ClassPaths.Add(UBlueprint::StaticClass()->GetClassPathName());
	Filter.bRecursivePaths = true;
	
	// Get the asset registry module
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<
		FAssetRegistryModule>("AssetRegistry");

	
	// Get all assets in the project
	TArray<FAssetData> AssetData;
	AssetRegistryModule.Get().GetAssets(Filter, AssetData);

	// Iterate through each asset
	TArray<FName> OutDependencies;
	TArray<FName> OutReferencers;
	FString TextFormat;
	FString OutLogText;
	for (const FAssetData& Asset : AssetData)
	{
		AssetRegistryModule.Get().GetDependencies(Asset.PackageName, OutDependencies,
		                                          UE::AssetRegistry::EDependencyCategory::Package);
		AssetRegistryModule.Get().GetReferencers(Asset.PackageName, OutReferencers,
		                                         UE::AssetRegistry::EDependencyCategory::Package);
		// Check if the asset has references
		if (OutReferencers.IsEmpty() && !OutDependencies.IsEmpty())
		{
			// Write asset information to the log file
			TSet<FName> VisitedAssets;
			LogAssetDependencies(OutLogText, Asset.PackageName, AssetRegistryModule, VisitedAssets, TextFormat);
		}
	}

	if (!OutLogText.IsEmpty())
	{
		// Create a log file
		const FString LogOutputPath = FPaths::Combine(FPaths::ProjectSavedDir(), "Logs",
		                                              FString::Printf(
			                                              TEXT("RefLogOutput_%s.log"), *FDateTime::Now().ToString()));
		FFileHelper::SaveStringToFile(OutLogText, *LogOutputPath);
		// Show a message dialog
		FMessageDialog::Open(EAppMsgType::Ok,
		                     FText::FromString(
			                     TEXT("Asset reference listing completed. Check the log file for details.")));
	}
	else
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("No references found.")));
}

void UBFLEditorUtils::LogAssetDependencies(FString& OutLogText, const FName& PackageName,
                                           const FAssetRegistryModule& AssetRegistryModule, TSet<FName>& VisitedAssets,
                                           FString& TextFormat, const int32 Counter)
{
	// Adjust format of text
	TextFormat = TEXT("");
	for (int i = 0; i < Counter; i++)
		TextFormat.Append(TEXT("   "));

	VisitedAssets.Add(PackageName);
	if (Counter != 0)
		OutLogText.Append(FString::Printf(TEXT("%s|- %s\n"), *TextFormat, *PackageName.ToString()));
	else
		OutLogText.Append(FString::Printf(TEXT("%s [ROOT]\n"), *PackageName.ToString()));

	//Check if asset has more dependencies
	TArray<FName> OutDependencies;
	AssetRegistryModule.Get().GetDependencies(PackageName, OutDependencies);

	// Iterate through dependencies
	for (const auto& AssetName : OutDependencies)
		if (!VisitedAssets.Contains(AssetName))
			//Recursive search
			LogAssetDependencies(OutLogText, AssetName, AssetRegistryModule, VisitedAssets, TextFormat, Counter + 1);
}



bool UBFLEditorUtils::GetAllAssetsInPathsByClass(const TSoftClassPtr<UObject>& SoftClassPtr,
                                                 TArray<FAssetData>& OutAssets)
{
	if (!SoftClassPtr)
	{
		const FNotificationInfo NotificationInfo{FText::FromString(TEXT("Class is not set..."))};
		FSlateNotificationManager& NotificationManager = FSlateNotificationManager::Get();
		NotificationManager.AddNotification(NotificationInfo);

		return false;
	}
	

	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(
		"AssetRegistry");
	const IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	TArray<FAssetData> AssetData;

	FARFilter Filter;
	Filter.ClassPaths.Add(SoftClassPtr->GetClassPathName());
	AssetRegistry.GetAssets(Filter, AssetData);

	Filter.SoftObjectPaths.Add(FString(TEXT("/Game")));

	Filter.bRecursivePaths = true;

	AssetRegistry.GetAssets(Filter, OutAssets);

	if (OutAssets.IsEmpty())
	{
		const FNotificationInfo NotificationInfo{FText::FromString(TEXT("No he encontrado ningún asset"))};
		FSlateNotificationManager& NotificationManager = FSlateNotificationManager::Get();
		NotificationManager.AddNotification(NotificationInfo);
		return false;
	}
	const FNotificationInfo NotificationInfo{
		FText::FromString(
			FString::Printf(TEXT("He encontrado %d assets de tipo %s"), OutAssets.Num(), *SoftClassPtr->GetName()))
	};
	FSlateNotificationManager& NotificationManager = FSlateNotificationManager::Get();
	NotificationManager.AddNotification(NotificationInfo);

	return true;
	//for(const auto& Path : InPath)
	//if(AssetRegistry.GetAssetsByPath(Path,AssetData))
	//{
	//	for(const auto& Asset : AssetData)
	//	{
	//		if(Asset.AssetClassPath == SMPath)
	//		{
	//			OutAssets.Add(Asset.GetAsset());
	//		}
	//	}
	//}
	//return !OutAssets.IsEmpty();
}

UTexture2D* UBFLEditorUtils::GetThumbnailFromAsset(const FAssetData& InAsset)
{
	// ThumbnailTools::
	return nullptr;
}


#endif
