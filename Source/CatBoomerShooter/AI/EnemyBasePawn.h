// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnemyBase.h"
#include "EnemyBasePawn.generated.h"

UCLASS()
class CATBOOMERSHOOTER_API AEnemyBasePawn : public APawn, public IEnemyBase
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyBasePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Static mesh component for pawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "default")
	class UStaticMeshComponent* PawnMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyBase")
	EEnemySize EnemySize = EEnemySize::Medium;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyBase")
	EEnemyType EnemyType = EEnemyType::Ranged;

	// Implement Interface Functions
	EEnemySize GetEnemySize_Implementation() override;
	EEnemyType GetEnemyType_Implementation() override;
};
