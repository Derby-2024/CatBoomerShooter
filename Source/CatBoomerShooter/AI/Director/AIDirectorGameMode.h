// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AIDirectorResources.h"
#include "AIDirectorGameMode.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTokenSystem, Log, All);

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

// Enemy Tracking stuff
private:
	// We may need to disable VisibleAnywhere at a later date if it starts lagging the editor
	UPROPERTY(VisibleAnywhere, Category = "EnemyTracking")
	FEnemyCollection Enemies;

public:
	UFUNCTION(BlueprintCallable, Category = "EnemyTracking")
	void RegisterEnemy(AActor* EnemyActor);

	UFUNCTION(BlueprintCallable, Category = "EnemyTracking")
	void RemoveEnemy(AActor* EnemyActor);

	/** Get all enemies. */
	UFUNCTION(BlueprintCallable, Category = "EnemyTracking")
	void GetEnemyActors(TArray<AActor*>& EnemyActors);

	/** Get all enemies of a given type. */
	UFUNCTION(BlueprintCallable, Category = "EnemyTracking")
	void GetEnemyActorsTyped(const EEnemyType EnemyType, TArray<AActor*>& EnemyActors);

	/** Get all enemies in range of origin.
	*	If setting origin to GetActorLocation, increase min radius to exclude that actor. */
	UFUNCTION(BlueprintCallable, Category = "EnemyTracking")
	void GetEnemyActorsInRange(const FVector Origin, const float MinRadius, const float MaxRadius, const TArray<AActor*> ActorsToIgnore, TArray<AActor*>& EnemyActors);

	/** Get all enemies of a given type in range of origin.
	*	If setting origin to GetActorLocation, increase min radius to exclude that actor. */
	UFUNCTION(BlueprintCallable, Category = "EnemyTracking")
	void GetEnemyActorsInRangeTyped(const FVector Origin, const float MinRadius, const float MaxRadius, const EEnemyType EnemyType, const TArray<AActor*> ActorsToIgnore, TArray<AActor*>& EnemyActors);



// Token Stuff
private:
	/** All current tokens and their assigned characters */
	UPROPERTY(VisibleAnywhere, Category = "Tokens")
	TMap<AActor*, FActorTokensCollection> ActorTokens;

public:
	/** Requests an enemy token of a given type. 
	*	Token Priority is currently unimplemented. */
	UFUNCTION(BlueprintCallable, Category = "Tokens")
	void RequestToken(class AAIEnemyBaseController* EnemyController, const AActor* TargetActor, const ETokenType TokenType, const ETokenPriority TokenPriority, UEnemyToken*& Token, bool& Success);

	/** Returns a token from being used by an enemy */
	UFUNCTION(BlueprintCallable, Category = "Tokens")
	void ReleaseToken(UEnemyToken* Token, const float CustomCooldown = -1.0f);

	/** Adds a set amount of tokens to an actor  */
	UFUNCTION(BlueprintCallable, Category = "Tokens")
	void AddTokensToActor(AActor* TargetActor, ETokenType TokenType, int Amount);

	/** Adds the default amount of tokens to an actor based on current difficulty */
	UFUNCTION(BlueprintCallable, Category = "Tokens")
	void AddDefaultTokensToActor(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "Tokens")
	void SetTokensLocked(AActor* TargetActor, bool Locked);

private:
	void TokenTimeout(UEnemyToken* Token);
	void TokenCooldownEnd(UEnemyToken* Token);

protected:
	virtual void StartPlay() override;
};
