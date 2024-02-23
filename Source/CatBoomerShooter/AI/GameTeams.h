// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameTeams.generated.h"

// Source:
// https://forums.unrealengine.com/t/how-do-i-use-the-ai-perception-teams/120837/2

/**
*
*/
UENUM(BlueprintType)
enum class EGameTeam : uint8
{
	Neutral,
	Aliens,
	Players
};

/**	Structure used in CustomProjectSettings to implement Array of Arrays (UE doesn't support 2d arrays)
*
*/
USTRUCT(BlueprintType)
struct FTeamAttitude
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TEnumAsByte<ETeamAttitude::Type>> Attitude;

	FTeamAttitude() {};

	FTeamAttitude(std::initializer_list<TEnumAsByte<ETeamAttitude::Type>> attitudes) :
		Attitude(std::move(attitudes)) {};
};
