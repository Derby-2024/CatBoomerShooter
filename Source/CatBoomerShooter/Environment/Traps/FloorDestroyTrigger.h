// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TriggerBox.h"
#include "FloorDestroyTrigger.generated.h"

/**
 * 
 */
UCLASS()
class CATBOOMERSHOOTER_API AFloorDestroyTrigger : public ATriggerBox
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay();

public:
	AFloorDestroyTrigger();

	UFUNCTION()
		void Event(class AActor* overlappedActor, class AActor* OtherActor);

	UPROPERTY(EditAnywhere)
		AActor* DestroyObject;
};
