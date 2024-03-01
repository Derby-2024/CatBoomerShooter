// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseAmmo.generated.h"

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	E_AssaultRifle		UMETA(DisplayName = "ASSAULT_RIFLE"),
	E_Shotgun			UMETA(DisplayName = "SHOTGUN"),
	E_SniperRifle		UMETA(DisplayName = "SNIPER_RIFLE"),
	E_Pistol			UMETA(DisplayName = "PISTOL"),
	E_Grenade			UMETA(DisplayName = "GRENADE")
};

USTRUCT(BlueprintType)
struct FAmmoStructX
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int AmmoAmount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int TotalAmmo = 0;
};

USTRUCT(BlueprintType)
struct FAmmoCollection
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FAmmoStructX ShotgunAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FAmmoStructX AssaultRifleAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FAmmoStructX PistolAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FAmmoStructX SniperAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FAmmoStructX GrenadeAmmo;

	FAmmoCollection();

	/** Get the correct FTokenCollection based on the enum value */
	FAmmoStructX* GetCollectionOfType(EAmmoType AmmoType);
};

USTRUCT(BlueprintType)
struct FAmmoStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int AmmoAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int TotalAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EAmmoType AmmoType;
};

UCLASS()
class CATBOOMERSHOOTER_API ABaseAmmo : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseAmmo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};