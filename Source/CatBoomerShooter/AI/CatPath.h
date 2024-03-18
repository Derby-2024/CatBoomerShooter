// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SplineMeshActor.h"
#include "CatPath.generated.h"

/**
 * 
 */
UCLASS()
class CATBOOMERSHOOTER_API ACatPath : public AActor
{


	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	class USplineComponent* SplinePath;
		
};
