// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../AIEnemyBaseController.h"
#include "AIDirectorResources.h"
#include "AIDirectorGameMode.generated.h"

// Forward Declaration until merge with branch
//class ABasePlayerCharacter;

/**
 * 
 */
UCLASS(ClassGroup = (AIDirector), Blueprintable, BlueprintType)
class CATBOOMERSHOOTER_API AAIDirectorGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	FEnemyToken BasicToken;
	TMap<ACharacter*, FEnemyTokenCollection> Tokens;

public:
	/** Requests an enemy token of a given type. */
	UFUNCTION(BlueprintCallable, Category = "Tokens")
	void RequestToken(const AAIEnemyBaseController* EnemyController, const AActor* TargetActor, const ETokenType TokenType, const ETokenPriority TokenPriority, FEnemyToken& Token, bool& Success);
	bool ReleaseToken();
};
