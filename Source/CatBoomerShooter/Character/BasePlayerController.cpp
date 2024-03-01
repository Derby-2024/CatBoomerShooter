// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "../AI/GameTeams.h"

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Add enhanced input to subsystem
	UEnhancedInputLocalPlayerSubsystem* Subsystem = 
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (Subsystem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Base player controller: Couldn't get Enhanced input local player subsystem."));
		return;
	}

	Subsystem->AddMappingContext(InputMappingContext, 0);
}

FGenericTeamId ABasePlayerController::GetGenericTeamId() const
{
	return uint8(EGameTeam::Players);
}
