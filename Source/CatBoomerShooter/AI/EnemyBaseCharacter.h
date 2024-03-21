// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.h"
#include "EnemyBaseCharacter.generated.h"

UCLASS()
class CATBOOMERSHOOTER_API AEnemyBaseCharacter : public ACharacter, public IEnemyBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyBase")
	EEnemySize EnemySize = EEnemySize::Medium;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyBase")
	EEnemyType EnemyType = EEnemyType::Ranged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyBase")
	float WalkSpeed = 800.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyBase")
	float RunSpeed = 1000.f;

	// Implement Interface Functions
	EEnemySize GetEnemySize_Implementation() override;
	EEnemyType GetEnemyType_Implementation() override;
	float GetMovementSpeed_Implementation(EMovementSpeed MovementSpeed) override;
};
