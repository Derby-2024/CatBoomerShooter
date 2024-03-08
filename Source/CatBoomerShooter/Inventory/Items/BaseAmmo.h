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
struct FAmmoStruct
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
	FAmmoStruct ShotgunAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FAmmoStruct AssaultRifleAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FAmmoStruct PistolAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FAmmoStruct SniperAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FAmmoStruct GrenadeAmmo;

	FAmmoCollection();

	/** Get the correct FTokenCollection based on the enum value */
	FAmmoStruct* GetCollectionOfType(EAmmoType AmmoType);
};
