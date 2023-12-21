#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyUtilityWidget.generated.h"

UCLASS()
class HERRAMIENTAS_API UMyUtilityWidget : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Utility Widget")
	static void AlignActors(TArray<AActor*> Actors);

	UFUNCTION(BlueprintCallable, Category = "Utility Widget")
	static void OrientToActor(TArray<AActor*> Actors, AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "Utility Widget")
	static void RandomizeActors(TArray<AActor*>& OutActors,AActor* CenterActor, TSubclassOf<AActor> ActorClass, int32 NumActors, float MinRange, float MaxRange);
	UFUNCTION(BlueprintCallable, Category = "Utility Widget")
	static void OrderActorsByDistance(TArray<AActor*>& Actors, const FVector& CenterPoint);
	
	UFUNCTION(BlueprintCallable, Category = "Utility Widget")
	static void SnapActors(AActor* ActorA, AActor* ActorB);

	UFUNCTION(BlueprintCallable, Category = "Utility Widget")
	static void AlignToFloor(TArray<AActor*> Actors, float DistanceFromFloor);

	UFUNCTION(BlueprintCallable, Category = "Utility Widget")
	static void MoveToLevel(TArray<AActor*>& Actors, FName LevelName);
	UFUNCTION(BlueprintCallable, Category = "Utility Widget")
	static void CreateWallFromActor(const AActor* OriginalActor, int32 NumColumns, int32 NumRows, int32 NumLayers,TArray<AActor*>& OutActors);

	UFUNCTION(BlueprintCallable, Category = "Utility Widget")
	static void ArrangeActorsInGrid(TArray<AActor*> Actors, const int32 NumColumns, const float Spacing);
	UFUNCTION(BlueprintCallable, Category = "Utility Widget")
	static void SetupLevel();

private:
	static FVector GetCentroid(const TArray<AActor*>& Actors);
};