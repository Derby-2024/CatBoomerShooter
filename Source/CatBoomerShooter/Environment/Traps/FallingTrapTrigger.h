// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/TriggerBox.h"
#include "FallingTrapTrigger.generated.h"

/**
 * 
 */
UCLASS()
class CATBOOMERSHOOTER_API AFallingTrapTrigger : public ATriggerBox
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay();

public:
	AFallingTrapTrigger();

	UFUNCTION()
		void Event(class AActor* overlappedActor, class AActor* OtherActor);

	UPROPERTY(EditAnywhere)
	//ball is what we want to interact with(this can be called anything)
		AActor* FallingObject;
};
