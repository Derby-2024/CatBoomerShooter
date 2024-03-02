// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "InteractableSwitch.generated.h"

/**
 * 
 */
UCLASS()
class CATBOOMERSHOOTER_API AInteractableSwitch : public AInteractableActor
{
	GENERATED_BODY()
	
private:
	void BeginPlay() override;

public:
	bool OnInteract_Implementation(AActor* OwningActor) override;

public:
	// Actor Reference that this switch will interact when toggled
	UPROPERTY(EditAnywhere)
	TArray<AActor*> LinkedActors;
};
