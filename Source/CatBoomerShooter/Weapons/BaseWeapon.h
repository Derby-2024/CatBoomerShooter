// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "Projectiles/BaseWeaponProjectile.h"
#include "BaseWeapon.generated.h"

UENUM(BlueprintType)
enum class EFiringMode : uint8
{
	Semi,
	Automatic,
	Burst
};

UCLASS()
class CATBOOMERSHOOTER_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

	//Total Amount of ammo being carried for the weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int TotalAmmo;

	//The amount of ammo in the weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int AmmoAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int CurrentAmmo;

	//The time it takes to reload weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float ReloadTime;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class USceneComponent* DefaultSceneComponent;
	UPROPERTY(EditAnywhere)
	class UArrowComponent* Arrow;
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	TSubclassOf<class ABaseWeaponProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	float FireRate = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	EFiringMode FiringMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	int BurstAmount = 0;
	
	int ShotsFired = 0;

	FTimerHandle Handle_ReFire;
	ACharacter* PlayerCharacter;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Fire();
	void BurstFire();
	void StartShooting();
	void StopShooting();

};