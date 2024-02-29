// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseAmmo.h"
#include "InventoryComponent.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Ammo,
	Weapon,
	Health,
	Collectible
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
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TArray<FAmmoStruct> Ammo;
};
