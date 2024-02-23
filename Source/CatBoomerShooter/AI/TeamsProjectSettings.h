// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameTeams.h"
#include "GenericTeamAgentInterface.h"
#include "TeamsProjectSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig)
class CATBOOMERSHOOTER_API UTeamsProjectSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category = "Teams")
	TArray<FTeamAttitude> TeamAttitudes;

public:
	UTeamsProjectSettings(const FObjectInitializer& ObjectInitializer);

	static const UTeamsProjectSettings* Get();

	UFUNCTION(BlueprintPure, Category = "Teams")
	static ETeamAttitude::Type GetAttitude(FGenericTeamId Of, FGenericTeamId Towards);
};
