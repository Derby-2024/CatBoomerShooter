// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "InteractableDoor.generated.h"

UENUM()
enum class EDoorState : uint8 {
	Opened,
	Closed,
};

UENUM()
enum class EDoorOpenMethod : uint8 {
	Rotate,
	Move,
};

/**
 * 
 */
UCLASS()
class CATBOOMERSHOOTER_API AInteractableDoor : public AInteractableActor
{
	GENERATED_BODY()
	
private:
	AInteractableDoor();

	void BeginPlay() override;
	
	FQuat InitialRotation;
	FVector InitialLocation;
	EDoorState TargetState = EDoorState::Closed;

public:
	void Tick(float DeltaTime) override;
	bool OnInteract_Implementation(AActor* OwningActor) override;

public:
	UPROPERTY(EditAnywhere)
	EDoorOpenMethod OpenMethod = EDoorOpenMethod::Rotate;

	UPROPERTY(EditAnywhere)
	float OpenSpeed = 3.0f;

	UPROPERTY(EditAnywhere)
	bool bCanBeClosed = true;

	UPROPERTY(EditAnywhere)
	bool bRequireKey = false;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bRequireKey"))
	FString RequiredKey = "KeyID";
};
