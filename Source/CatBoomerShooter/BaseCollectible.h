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
struct FCollectibleStructX
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
	FCollectibleStructX Cats;

	FCollectibles();

	/** Get the correct FTokenCollection based on the enum value */
	FCollectibleStructX* GetCollectionOfType(ECollectibleType CollectibleType);
};


USTRUCT(BlueprintType)
struct FCollectibleStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collectible")
	FString ItemInformation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectible")
	int CatCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectible")
	int TotalCats = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectible")
	ECollectibleType CollectibleType;
};

UCLASS()
class CATBOOMERSHOOTER_API ABaseCollectible : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseCollectible();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
