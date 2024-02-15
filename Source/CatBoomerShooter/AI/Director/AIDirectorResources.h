// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIDirectorResources.generated.h"

// Token system references
// https://www.gamedeveloper.com/design/cyber-demons-the-ai-of-doom-2016-
// https://youtu.be/2KQNpQD8Ayo?si=zc6b-AJQagwvrBvr&t=2503

/**
*
*/
UENUM(BlueprintType)
enum class ETokenType : uint8
{
	Light,
	Heavy
	//Boss
};

/**
*
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
};

/**
*
*/
USTRUCT(BlueprintType)
struct CATBOOMERSHOOTER_API FEnemyTokenCollection
{
	GENERATED_BODY()

	TMap<int, FEnemyToken> NormalTokens;
	TMap<int, FEnemyToken> HeavyTokens;
};
