// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../AIEnemyBaseController.h"
#include "AIDirectorResources.h"
#include "AIDirectorGameMode.generated.h"

const float TOKEN_TIMEOUT = 10.0f;
// Change this to a difficulty option later
const float TOKEN_COOLDOWN = 1.5f;

/**
 * 
 */
UCLASS(ClassGroup = (AIDirector), Blueprintable, BlueprintType)
class CATBOOMERSHOOTER_API AAIDirectorGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	/** All current tokens and their assigned characters */
	UPROPERTY(VisibleAnywhere, Category = "Tokens")
	TMap<AActor*, FActorTokensCollection> ActorTokens;

public:
	/** Requests an enemy token of a given type. 
	*	Token Priority is currently unimplemented. */
	UFUNCTION(BlueprintCallable, Category = "Tokens")
	void RequestToken(AAIEnemyBaseController* EnemyController, const AActor* TargetActor, const ETokenType TokenType, const ETokenPriority TokenPriority, UEnemyToken*& Token, bool& Success);

	/** Returns a token from being used by an enemy */
	UFUNCTION(BlueprintCallable, Category = "Tokens")
	void ReleaseToken(UEnemyToken* Token, const float CustomCooldown = -1.0f);

	/** Adds a set amount of tokens to an actor  */
	UFUNCTION(BlueprintCallable, Category = "Tokens")
	void AddTokensToActor(AActor* TargetActor, ETokenType TokenType, int Amount);

	/** Adds the default amount of tokens to an actor based on current difficulty */
	UFUNCTION(BlueprintCallable, Category = "Tokens")
	void AddDefaultTokensToActor(AActor* TargetActor);

private:
	void TokenTimeout(UEnemyToken* Token);
	void TokenCooldownEnd(UEnemyToken* Token);

public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

protected:
	virtual void BeginPlay() override;
};
