// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PatrolComponent.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct CATBOOMERSHOOTER_API FPatrolData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class APatrolPoint* PatrolPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PatrolPointRange = 50.0f;

	// Use Patrol Point Rotation
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool UsePatrolPointRotation = true;

	// Random angle variance when facing patrol point direction on arival
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RotationVariance = 10.f;

	// Wait Time
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float WaitTime = 1.0f;

	// Wait Time Variance
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float WaitTimeVariance = 0.0f;
};

/**
*
*/
UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATBOOMERSHOOTER_API UPatrolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPatrolComponent();

	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol")
	TArray<FPatrolData> PatrolPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol")
	int CurrentPatrolIndex = 0;

	UFUNCTION(BlueprintCallable)
	void GetCurrentPatrolPoint(FPatrolData& PatrolData, bool& IsValid);

	UFUNCTION(BlueprintCallable)
	void IterPatrolPointIndex();

	UFUNCTION(BlueprintCallable)
	void SetPatrolPointIndex(int NewIndex);
	
};
