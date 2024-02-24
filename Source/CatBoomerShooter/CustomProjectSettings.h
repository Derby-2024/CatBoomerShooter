// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "AI/GameTeams.h"
#include "GenericTeamAgentInterface.h"
#include "CustomProjectSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig)
class CATBOOMERSHOOTER_API UCustomProjectSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category = "Teams")
	TArray<FTeamAttitude> TeamAttitudes;

public:
	UCustomProjectSettings(const FObjectInitializer& ObjectInitializer);

	static const UCustomProjectSettings* Get();

	UFUNCTION(BlueprintPure, Category = "Teams")
	static ETeamAttitude::Type GetAttitude(FGenericTeamId Of, FGenericTeamId Towards);
};
