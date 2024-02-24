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

 /**
  *
  */
class IEnemyBase
{
	 GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "EnemyData")
	int GetEnemyHealth();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "EnemyData")
	EEnemySize GetEnemySize();
};
