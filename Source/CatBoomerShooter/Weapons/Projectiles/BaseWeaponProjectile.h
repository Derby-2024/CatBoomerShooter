// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "EMSActorSaveInterface.h"
#include "BaseWeaponProjectile.generated.h"

UCLASS()
class CATBOOMERSHOOTER_API ABaseWeaponProjectile : public AActor, public IEMSActorSaveInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeaponProjectile();

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* ProjectileMovement;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* BulletMesh;

	//Default Values
	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite, Category = "Defaults")
	float Damage = 0.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
