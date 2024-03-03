// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BaseWeaponProjectile.generated.h"

UCLASS()
class CATBOOMERSHOOTER_API ABaseWeaponProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeaponProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* BulletMesh;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* ProjectileMovement;


	//Default Values
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	float Damage = 0.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

};
