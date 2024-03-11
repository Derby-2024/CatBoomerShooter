// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ShapeComponent.h"
#include "Components/SplineComponent.h"
#include "Components/BoxComponent.h"
#include "../CatBoomerShooter/Character/BasePlayerCharacter.h"
#include "Train.generated.h"

UCLASS()
class CATBOOMERSHOOTER_API ATrain : public AActor
{
	GENERATED_BODY()
	
public:	
	ATrain();

	UPROPERTY(EditAnywhere)
	float RandomDelay;


protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Train", meta=(AllowPrivateAccess = "true"))
	USplineComponent* SplineComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Train", meta = (AllowPrivateAccess = "true"))
	UShapeComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Train", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TrainMesh;

	UPROPERTY(EditAnywhere)
	float TrainSpeed;

	UPROPERTY(EditAnywhere)
	float DamageAmount;

	UPROPERTY(EditAnywhere)
	float PushForce;

	bool bIsTeleportTimerActive;

	UFUNCTION()
	void OnTrainOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	FVector NextTargetLocation;
	FVector StartLocation;
	FTimerHandle TeleportDelay;


	void SetNextTargetPoint();
	void MoveTowardsTarget();
	void TeleportTrain();
};
