// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "../Inventory/InventoryComponent.h"
#include "../Inventory/Items/BaseAmmo.h"
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class USceneComponent* DefaultSceneComponent;
	UPROPERTY(EditAnywhere)
	class UArrowComponent* Arrow;
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	TSubclassOf<class ABaseWeaponProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	float FireRate = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	EFiringMode FiringMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	int BurstAmount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	UTexture* GunImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	TArray<FName> MuzzleSockets = {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	float MaxVerticalSpread = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	float MaxHorizontalSpread = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	int NumberOfPellets = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	bool needsReload = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	FName HandSocketName = "R_HandSocket";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	APawn* OwningPawn;

	int ShotsFired = 0;

	UPROPERTY(EditAnywhere, Category = "Collision")
	TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECC_Pawn;

	FTimerHandle Handle_ReFire;
	FTimerHandle Handle_Reload;

	FVector TraceStart;
	FVector TraceEnd;

	bool canShoot = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	float ReloadTime = 1.0f;



public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void StartShooting();
	UFUNCTION(BlueprintCallable)
	void StopShooting();

	void Fire();
	void Reload();
	void ResetShot();

	UFUNCTION(BlueprintNativeEvent, Category = "Reloading")
	void BPEnableReloadWidget();
	UFUNCTION(BlueprintNativeEvent, Category = "Reloading")
	void BPDisableReloadWidget();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	USkeletalMeshComponent* GetWeaponMesh();

	FRotator RandomSpread(FRotator spawnRotation, float maxVertical, float maxHorizontal);

};