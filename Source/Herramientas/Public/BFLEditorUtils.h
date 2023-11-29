// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/Object.h"
#include "BFLEditorUtils.generated.h"

/**
 * 
 */
UCLASS()
class HERRAMIENTAS_API UBFLEditorUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

#if WITH_EDITOR
	UFUNCTION(BlueprintCallable)
	static void GetCDOFromBlueprintAsset(const UObject* InBlueprintAsset, UObject*& OutCDO);

	UFUNCTION(BlueprintCallable)
	static void ListTagAndValues(const FAssetData& InAssetData);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static bool IsPowerOfTwo(int32 InValue);

	UFUNCTION(BlueprintPure)
	static void GetExportsPath(FString& OutPath);

	//TSoftObjectPtr<UObject> SoftObjectPtr;
	//TSoftClassPtr<UObject> SoftClassPtr;
	UFUNCTION(BlueprintCallable)
	static bool GetAllAssetsOfClass(const UClass* InClass, TArray<FAssetData>& OutAssetData);
	UFUNCTION(BlueprintCallable, Category = "Reference Checker")
	static void ListAssetReferences();
	static void LogAssetDependencies(FString& OutLogText, const FName& PackageName,
	                                 const FAssetRegistryModule& AssetRegistryModule, TSet<FName>& VisitedAssets, FString& TextFormat, const int32 Counter = 0);
	UFUNCTION(BlueprintCallable)
		static bool GetAllAssetsInPathsByClass(const TSoftClassPtr<UObject>& SoftClassPtr , TArray<FAssetData>& OutAssets );
	UFUNCTION(BlueprintCallable)
	static UTexture2D* GetThumbnailFromAsset(const FAssetData& InAsset);

#endif
};
