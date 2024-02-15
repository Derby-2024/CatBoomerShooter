// Fill out your copyright notice in the Description page of Project Settings.


#include "AIEnemyBaseController.h"
#include "Navigation/CrowdFollowingComponent.h"

AAIEnemyBaseController::AAIEnemyBaseController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	if (DefaultBehaviorTree != nullptr)
	{
		if (!RunBehaviorTree(DefaultBehaviorTree))
		{
			UE_LOG(LogTemp, Warning, TEXT("AIEnemyBaseController Constructor: Failed to run provided default behavior tree."));
		}
	}
}
