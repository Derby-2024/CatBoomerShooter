// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIEnemyBaseController.generated.h"

/**
 * 
 */
UCLASS()
class CATBOOMERSHOOTER_API AAIEnemyBaseController : public AAIController
{
	GENERATED_BODY()

public:
	AAIEnemyBaseController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, Category = "EnemyBase")
	UBehaviorTree* DefaultBehaviorTree;
};

