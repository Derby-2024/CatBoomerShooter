// Fill out your copyright notice in the Description page of Project Settings.


#include "AIEnemyBaseController.h"
#include "Navigation/CrowdFollowingComponent.h"

AAIEnemyBaseController::AAIEnemyBaseController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{

}
