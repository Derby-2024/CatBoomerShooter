// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.generated.h"

/**
 *
 */
UENUM(BlueprintType)
enum class EEnemySize : uint8 {
	Small,
	Medium,
	Large
};

/**
 * 
 */
 UINTERFACE(Blueprintable)
class UEnemyBase : public UInterface
{
	GENERATED_BODY()
};

/**	Base Enemy Type
*	All enemy types should inherit this. Most enemies will inherit from EnemyBaseCharacter
*	which already inherits this. Any non-character enemies should make sure to implement this.
*/	
class IEnemyBase
{
	 GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "EnemyData")
	EEnemySize GetEnemySize();
};
