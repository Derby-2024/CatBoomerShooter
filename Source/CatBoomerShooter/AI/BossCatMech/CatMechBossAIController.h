// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../AIEnemyBaseController.h"
#include "CatMechBossAIController.generated.h"

/**
 * 
 */
UCLASS()
class CATBOOMERSHOOTER_API ACatMechBossAIController : public AAIEnemyBaseController
{
	GENERATED_BODY()

protected:
	FRotator SmoothTargetRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SmoothFocusInterpSpeed = 30.0f;

public:
	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn) override;
	
};
