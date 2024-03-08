// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseCollectible.generated.h"


UENUM(BlueprintType)
enum class ECollectibleType : uint8
{
	E_Cat		UMETA(DisplayName = "CAT")
};

USTRUCT(BlueprintType)
struct FCollectibleData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collectible")
	FString ItemInformation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectible")
	int CatCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectible")
	int TotalCats = 10;
}; 

USTRUCT(BlueprintType)
struct FCollectibles
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectible")
	FCollectibleData Cats;

	FCollectibles();

	/** Get the correct FTokenCollection based on the enum value */
	FCollectibleData* GetCollectionOfType(ECollectibleType CollectibleType);
};
