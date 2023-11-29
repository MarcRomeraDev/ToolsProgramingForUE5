// Fill out your copyright notice in the Description page of Project Settings.


#include "ConditionLibrary.h"
#include "AssetToolsModule.h"
#include "HAL/FileManagerGeneric.h"
#include "Kismet/KismetSystemLibrary.h"

#if WITH_EDITOR
bool UConditionLibrary::CheckTriCount(const UStaticMesh* StaticMesh)
{
	return StaticMesh->GetNumTriangles(0) <= 10000;
}

bool UConditionLibrary::CheckMaterialSlots(const UStaticMesh* StaticMesh)
{
	return StaticMesh->GetStaticMaterials().Num() <= 5;
}

bool UConditionLibrary::CheckAbsoluteSpace(const UStaticMesh* StaticMesh)
{
	return StaticMesh->GetBounds().GetSphere().W * 2 <= 6000;
}

void UConditionLibrary::CheckConditionsForStaticMeshes(TArray<FString>& OutMeshesName,
                                                       TArray<AStaticMeshActor*> InStaticMeshes)
{

	TCondChecker<const UStaticMesh*> CondChecker(
		CheckTriCount,
		CheckMaterialSlots,
		CheckAbsoluteSpace);

	for (const auto& StaticMeshActor : InStaticMeshes)
	{
		const UStaticMesh* StaticMeshAsset = StaticMeshActor->GetStaticMeshComponent()->GetStaticMesh();
		const auto result = CondChecker(MoveTempIfPossible(StaticMeshAsset));
		if (!result.first)
		{
			OutMeshesName.Add(StaticMeshAsset->GetPackage()->GetName());
		}
	}
}

bool UConditionLibrary::ExportStaticMeshes(const TArray<FString>& PackageNames)
{
	const FString FolderPath = UKismetSystemLibrary::GetProjectDirectory() + "Saved/Rework";
	FFileManagerGeneric FManager = FFileManagerGeneric();
	FManager.MakeDirectory(*FolderPath, false);
	
	const FAssetToolsModule& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");
	AssetToolsModule.Get().ExportAssets(PackageNames, *FolderPath);
	
	return true;
}
#endif
