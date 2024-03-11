// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/BaseAmmo.h"
#include "Items/BaseCollectible.h"
#include "EMSCompSaveInterface.h"
#include "InventoryComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogInventory, Log, All);

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Ammo,
	Weapon,
	Health,
	Collectible,
	Key
};

USTRUCT(BlueprintType)
struct FItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int AmmoAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int TotalAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString ItemInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int TotalCats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int CatCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString CollectibleInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	ECollectibleType CollectibleType;
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CATBOOMERSHOOTER_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool AddItem(const FItem& Item);
	bool RemoveItem(const FItem& Item);

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadOnly, Category = "Item")
	FAmmoCollection Ammo;

	UPROPERTY(EditAnywhere, SaveGame, Category = "Item")
	TArray<FString> Keys;

	UPROPERTY(EditAnywhere, SaveGame, Category = "Item")
	FCollectibles Collectibles;

public:
	UFUNCTION(BlueprintCallable)
	FAmmoContainer GetAmmoCollectionOfType(EAmmoType AmmoType);
};