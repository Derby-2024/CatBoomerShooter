// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ShapeComponent.h"
#include "Components/SplineComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "../CatBoomerShooter/Character/BasePlayerCharacter.h"
#include "Train.generated.h"

UCLASS()
class CATBOOMERSHOOTER_API ATrain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
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

	bool bMovingForward;

	UFUNCTION()
	void OnTrainOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	FVector NextTargetLocation;
	FTimerHandle MoveTimerHandle;

	void SetNextTargetPoint();
	void MoveTowardsTarget();
};
