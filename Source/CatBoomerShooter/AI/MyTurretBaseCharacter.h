// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBaseCharacter.h"
#include "MyTurretBaseCharacter.generated.h"

/**
 * 
 */
UCLASS()
class CATBOOMERSHOOTER_API AMyTurretBaseCharacter : public AEnemyBaseCharacter
{
	GENERATED_BODY()

public:

	void GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const override;
	
};
