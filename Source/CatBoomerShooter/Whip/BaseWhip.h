// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "BaseWhip.generated.h"

UCLASS()
class CATBOOMERSHOOTER_API ABaseWhip : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWhip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class USceneComponent* DefaultSceneComponent;
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* Whip_SK;
	UPROPERTY(EditAnywhere)
	class UArrowComponent* Arrow;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* AttackOneCollision;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* AttackTwoCollision;

	//Default Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	bool isAttacking = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	float Damage = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	TArray<ACharacter*> EnemiesHit = {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	int KnockbackPower = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	int AttackCombo = 1;

	ACharacter* PlayerCharacter;
	FVector LaunchVelocity;
	FTimerHandle CountdownTimer;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Attack();
	void AttackFinished();
	void ResetCombo();

};
