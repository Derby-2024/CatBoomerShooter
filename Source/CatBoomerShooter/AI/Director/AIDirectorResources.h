// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIDirectorResources.generated.h"

// Forward declare EnemyBase.h enum
enum class EEnemyType : uint8;

USTRUCT(BlueprintType)
struct CATBOOMERSHOOTER_API FEnemyCollection
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AActor*> Enemies;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AActor*> SwarmEnemies;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AActor*> RangedEnemies;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AActor*> TankEnemies;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AActor*> BossEnemies;

	/** Get the correct array based on the enum value */
	TArray<AActor*>* GetCollectionOfType(EEnemyType EnemyType);
};


// Token system references
// https://www.gamedeveloper.com/design/cyber-demons-the-ai-of-doom-2016-
// https://youtu.be/2KQNpQD8Ayo?si=zc6b-AJQagwvrBvr&t=2503

/** Type of attack a token is used for.
*	Currently just a concept.
*/
UENUM(BlueprintType)
enum class ETokenType : uint8
{
	Normal,
	Heavy
	//Boss
};

/** Low priority tokens may be able to be taken by enemies with
*	high priority.
*	Currently just a concept.
*/
UENUM(BlueprintType)
enum class ETokenPriority : uint8
{
	Low,
	High
};

/**
* 
*/
UCLASS(BlueprintType)
class CATBOOMERSHOOTER_API UEnemyToken : public UObject
{
	GENERATED_BODY()

public:
	// Will usually be a player 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ETokenType TokenType;

	// Enemy/Actor who is using the token
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AAIEnemyBaseController* ClaimedBy;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ETokenPriority ClaimPriority;

	FTimerHandle TimerHandle;
};

/**
*
*/
USTRUCT(BlueprintType)
struct CATBOOMERSHOOTER_API FTokenCollection
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UEnemyToken*> FreeTokens;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UEnemyToken*> ClaimedTokens;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UEnemyToken*> CooldownTokens;
};

/**
*
*/
USTRUCT(BlueprintType)
struct CATBOOMERSHOOTER_API FActorTokensCollection
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTokenCollection NormalTokens;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTokenCollection HeavyTokens;

	/** Get the correct FTokenCollection based on the enum value */
	FTokenCollection* GetCollectionOfType(ETokenType TokenType);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool Locked = false;
};
