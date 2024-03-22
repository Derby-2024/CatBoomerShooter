// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../EnemyBasePawn.h"
#include "MyTurretBasePawn.generated.h"


/**
 * 
 */
UCLASS()
class CATBOOMERSHOOTER_API AMyTurretBasePawn : public AEnemyBasePawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyTurretBasePawn();
	void GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const override;

protected:
	// Static mesh component for Turrets eye level
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EyeMesh")
	class UStaticMeshComponent* EyeMesh;

};