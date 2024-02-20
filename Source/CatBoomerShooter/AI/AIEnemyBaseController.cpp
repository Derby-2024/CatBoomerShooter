// Fill out your copyright notice in the Description page of Project Settings.


#include "AIEnemyBaseController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Director/AIDirectorGameMode.h"
#include "Kismet/GameplayStatics.h"

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

void AAIEnemyBaseController::RequestToken(const AActor* TargetActor, const ETokenType TokenType, const ETokenPriority TokenPriority, UEnemyToken*& Token, bool& Success)
{
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	if (AAIDirectorGameMode* AIDirector = Cast<AAIDirectorGameMode>(GameMode))
	{
		AIDirector->RequestToken(this, TargetActor, TokenType, TokenPriority, Token, Success);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("AAIEnemyBaseController::RequestToken: Could not get AIDirector Gamemode from current Gamemode."));
}

void AAIEnemyBaseController::ReleaseToken(UEnemyToken* Token, const float CustomCooldown)
{
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	if (AAIDirectorGameMode* AIDirector = Cast<AAIDirectorGameMode>(GameMode))
	{
		AIDirector->ReleaseToken(Token, CustomCooldown);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("AAIEnemyBaseController::ReleaseToken: Could not get AIDirector Gamemode from current Gamemode."));
}
