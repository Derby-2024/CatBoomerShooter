// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "InteractableDoor.h"
#include "InteractableDoorMesh.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class CATBOOMERSHOOTER_API UInteractableDoorMesh : public UStaticMeshComponent
{
	GENERATED_BODY()

private:
	void BeginPlay() override;

private:
	FQuat InitialRotation;
	FVector InitialLocation;

public:
	UPROPERTY(EditAnywhere)
	FVector OpenDelta = FVector(0, 0, 90);

public:
	const FQuat GetTargetRotation(EDoorState TargetState);
	const FVector GetTargetLocation(EDoorState TargetState);
	
};
