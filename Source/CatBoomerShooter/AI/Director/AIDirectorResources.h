// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../AIEnemyBaseController.h"
#include "AIDirectorResources.generated.h"

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
USTRUCT(BlueprintType)
struct CATBOOMERSHOOTER_API FEnemyToken
{
	GENERATED_BODY()

	// Will usually be a player 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ETokenType TokenType;

	// Enemy/Actor who is using the token
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AAIEnemyBaseController* ClaimedBy;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ETokenPriority ClaimPriority;

	// Used to manage token cooldown and timeout - Todo
	// UPROPERTY(VisibleAnywhere)
	// FTimerHandle TimerHandle;
};

/**
*
*/
USTRUCT(BlueprintType)
struct CATBOOMERSHOOTER_API FTokenCollection
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FEnemyToken> FreeTokens;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FEnemyToken> ClaimedTokens;
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
};
