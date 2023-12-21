// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUtilityWidget.h"

#include "Chaos/Utilities.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Engine/StaticMeshActor.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "Kismet/GameplayStatics.h"

void UMyUtilityWidget::OrderActorsByDistance(TArray<AActor*>& Actors, const FVector& CenterPoint)
{
	// Sort the array based on the distance to the center point
	Actors.Sort([CenterPoint](const AActor& A, const AActor& B)
	{
		return FVector::DistSquared(A.GetActorLocation(), CenterPoint) > FVector::DistSquared(
			B.GetActorLocation(), CenterPoint); // Sort in descending order
	});
}

FVector UMyUtilityWidget::GetCentroid(const TArray<AActor*>& Actors)
{
	FVector Centroid = FVector::ZeroVector;

	for (const AActor* Actor : Actors)
	{
		if (Actor)
		{
			Centroid += Actor->GetActorLocation();
		}
	}

	if (Actors.Num() > 0)
	{
		Centroid /= Actors.Num();
	}

	return Centroid;
}

void UMyUtilityWidget::AlignActors(TArray<AActor*> Actors)
{
	FVector StartPoint = FVector::Zero();
	FVector EndPoint = FVector::Zero();

	OrderActorsByDistance(Actors, GetCentroid(Actors));
	StartPoint = Actors[0]->GetActorLocation();
	EndPoint = Actors[1]->GetActorLocation();

	FVector LineDirVector = (EndPoint - StartPoint);
	for (const auto Actor : Actors)
	{
		FVector ToActorDirVector = Actor->GetActorLocation() - StartPoint;
		LineDirVector.Normalize(0.00001f);
		Actor->SetActorLocation(StartPoint + LineDirVector * FVector::DotProduct(LineDirVector, ToActorDirVector));
	}
}

void UMyUtilityWidget::OrientToActor(TArray<AActor*> Actors, AActor* TargetActor)
{
	if (TargetActor)
	{
		for (AActor* Actor : Actors)
		{
			if (Actor && Actor != TargetActor)
			{
				//FVector LookAtDirection = TargetActor->GetActorLocation() - Actor->GetActorLocation();
				//LookAtDirection.Normalize();
				//FQuat NewRotationQuat = FQuat::FindBetweenVectors(FVector::ForwardVector, LookAtDirection);
				//FRotator NewRotation = FQuatRotationMatrix(NewRotationQuat).Rotator();

				//matriz de rotacion basada en el vector direccion
				const FRotator NewRotation = FRotationMatrix::MakeFromX(
					TargetActor->GetActorLocation() - Actor->GetActorLocation()).Rotator();
				// convierte esa matriz en una rotación thanks to Rotator

				// Aplica la nueva rotación al actor
				Actor->SetActorRotation(NewRotation);
			}
		}
	}
}

void UMyUtilityWidget::RandomizeActors(TArray<AActor*>& OutActors, AActor* CenterActor, TSubclassOf<AActor> ActorClass,
                                       int32 NumActors, float MinRange, float MaxRange)
{
	if (CenterActor && ActorClass)
	{
		if (UWorld* World = CenterActor->GetWorld())
		{
			for (int32 i = 0; i < NumActors; ++i)
			{
				// Calcula una posición aleatoria en un radio específico
				const float RandomRadius = FMath::RandRange(MinRange, MaxRange);
				const FVector2D RandomLocation = FMath::RandPointInCircle(RandomRadius);

				// Calcula una rotación aleatoria
				FRotator RandomRotation = FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f);

				// Calcula una escala aleatoria
				FVector RandomScale = FVector(FMath::RandRange(0.5f, 2.0f));

				// Aplica la transformación al actor
				FTransform SpawnTransform(RandomRotation, CenterActor->GetActorLocation() + FVector(RandomLocation, 0),
				                          RandomScale);

				// Spawnea el actor
				AActor* SpawnedActor = World->SpawnActor<AActor>(ActorClass, SpawnTransform);

				if (SpawnedActor)
				{
					// cualquier otra configuración necesaria para el actor recién creado
				}
			}
		}
	}
}

void UMyUtilityWidget::SnapActors(AActor* ActorA, AActor* ActorB)
{
	if (!ActorA || !ActorB)
		return;

	const UPrimitiveComponent* MeshA = ActorA->FindComponentByClass<UPrimitiveComponent>();
	const UPrimitiveComponent* MeshB = ActorB->FindComponentByClass<UPrimitiveComponent>();

	if (!MeshA || !MeshB)
		return;

	const FBox BoxA = MeshA->Bounds.GetBox();
	const FBox BoxB = MeshB->Bounds.GetBox();

	// Obtiene los vértices de las cajas delimitadoras
	UE::Math::TVector<double> VerticesA[8];
	UE::Math::TVector<double> VerticesB[8];

	BoxA.GetVertices(VerticesA);
	BoxB.GetVertices(VerticesB);

	FVector ClosestVertexA;
	FVector ClosestVertexB;
	float MinDistanceSquared = TNumericLimits<float>::Max(); // Bastante chulo esto haha

	for (const FVector& VertexA : VerticesA)
	{
		for (const FVector& VertexB : VerticesB)
		{
			if (const float DistanceSquared = FVector::DistSquared(VertexA, VertexB); DistanceSquared <
				MinDistanceSquared)
			{
				MinDistanceSquared = DistanceSquared;
				ClosestVertexA = VertexA;
				ClosestVertexB = VertexB;
			}
		}
	}

	// Mueve ActorB para que su vértice coincida con ClosestVertexA
	ActorB->SetActorLocation(ClosestVertexA - (ClosestVertexB - ActorB->GetActorLocation()));

	// También podrías mover ActorA para que su vértice coincida con ClosestVertexB si es necesario
	// ActorA->SetActorLocation(ClosestVertexB);
}

void UMyUtilityWidget::AlignToFloor(TArray<AActor*> Actors, float DistanceFromFloor)
{
	if (Actors.IsEmpty())
		return;
	UWorld* World = GEngine->GetWorld();
	for (AActor* Actor : Actors)
	{
		World = Actor->GetWorld();
		if (!Actor)
		{
			continue;
		}

		UPrimitiveComponent* Mesh = Actor->FindComponentByClass<UPrimitiveComponent>();

		if (Mesh)
		{
			FBox Box = Mesh->Bounds.GetBox();
			FVector StartLocation = Actor->GetActorLocation();
			FVector EndLocation = FVector(StartLocation.X, StartLocation.Y, StartLocation.Z - 1000.0f);
			// Punto por debajo del suelo
			FHitResult HitResult;


			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(Actor);

			if (World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams))
			{
				FVector NewLocation = Actor->GetActorLocation();

				NewLocation.Z = HitResult.Location.Z + DistanceFromFloor;

				Actor->SetActorLocation(NewLocation);

				DrawDebugLine(World, StartLocation, HitResult.Location, FColor::Green, false, 1, 0, 1);
			}
		}
	}
}

void UMyUtilityWidget::MoveToLevel(TArray<AActor*>& Actors, FName LevelName)
{
	if (Actors.IsEmpty())
		return;
	UWorld* World = Actors[0]->GetWorld();

	if (!World)
		return;
	bool result = false;
	ULevelStreamingDynamic* StreamingLevel = ULevelStreamingDynamic::LoadLevelInstance(
		World, LevelName.ToString(), FVector::Zero(), FRotator::ZeroRotator, result);

	if (!StreamingLevel)
		return;

	for (AActor* Actor : Actors)
	{
		if (Actor)
		{
			Actor->GetRootComponent()->SetIsReplicated(false);
			Actor->GetRootComponent()->SetMobility(EComponentMobility::Movable);
			Actor->GetRootComponent()->SetWorldLocation(
				Actor->GetRootComponent()->GetComponentLocation() + StreamingLevel->LevelTransform.GetTranslation());
		}
	}
}


void UMyUtilityWidget::CreateWallFromActor(const AActor* OriginalActor, const int32 NumColumns, const int32 NumRows,
                                           const int32 NumLayers, TArray<AActor*>& OutActors)
{
	if (!OriginalActor || NumColumns <= 0 || NumRows <= 0 || NumLayers <= 0)
		return;

	UWorld* World = OriginalActor->GetWorld();
	if (!World)
		return;

	const UStaticMeshComponent* OriginalMeshComponent = OriginalActor->FindComponentByClass<UStaticMeshComponent>();
	if (!OriginalMeshComponent)
		return;

	UStaticMesh* OriginalStaticMesh = OriginalMeshComponent->GetStaticMesh();
	if (!OriginalStaticMesh)
		return;

	const FBox OriginalBoundingBox = OriginalStaticMesh->GetBoundingBox();
	const FVector CellSize = OriginalBoundingBox.GetSize() * OriginalActor->GetActorScale3D();

	// Crear el muro
	for (int32 LayerIndex = 0; LayerIndex < NumLayers; ++LayerIndex)
	{
		for (int32 RowIndex = 0; RowIndex < NumRows; ++RowIndex)
		{
			for (int32 ColIndex = 0; ColIndex < NumColumns; ++ColIndex)
			{
				// Calcular la posición para cada copia del actor
				FVector SpawnLocation = OriginalActor->GetActorLocation() +
					FVector(
						ColIndex * CellSize.Y,
						RowIndex * CellSize.X,
						LayerIndex * CellSize.Z);

				// Crear una copia del actor original
				if (AStaticMeshActor* SpawnedActor = World->SpawnActor<AStaticMeshActor>(
					OriginalActor->GetClass(), SpawnLocation, OriginalActor->GetActorRotation()))
				{
					SpawnedActor->SetActorRelativeScale3D(OriginalActor->GetActorRelativeScale3D());
					SpawnedActor->GetStaticMeshComponent()->SetStaticMesh(OriginalStaticMesh);
					OutActors.Add(SpawnedActor);
					// Para propósitos de depuración, dibuja una caja alrededor de cada copia
					DrawDebugBox(World, SpawnLocation, CellSize / 2, FColor::Green, false, -1, 0, 1);
				}
			}
		}
	}
}

void UMyUtilityWidget::ArrangeActorsInGrid(TArray<AActor*> Actors, const int32 NumColumns, const float Spacing)
{
	if (Actors.Num() == 0 || NumColumns <= 0)
		return;

	int32 NumRows = FMath::CeilToInt(static_cast<float>(Actors.Num()) / static_cast<float>(NumColumns));
	const FVector Centroid = GetCentroid(Actors);
	for (int32 Index = 0; Index < Actors.Num(); ++Index)
	{
		if (Actors[Index] && Actors[Index]->IsValidLowLevel())
		{
			const int32 RowIndex = Index / NumColumns;
			const int32 ColIndex = Index % NumColumns;

			FVector GridLocation = Centroid + FVector(ColIndex * Spacing, RowIndex * Spacing, 0.0f);

			Actors[Index]->SetActorLocation(GridLocation);
		}
	}
}

void UMyUtilityWidget::SetupLevel()
{
	// Implementar la lógica para configurar el nivel
}
