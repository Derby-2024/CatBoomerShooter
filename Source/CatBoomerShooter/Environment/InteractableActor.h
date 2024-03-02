// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractInterface.h"
#include "InteractableActor.generated.h"

UCLASS()
class CATBOOMERSHOOTER_API AInteractableActor : public AActor, public IInteract
{
	GENERATED_BODY()
	
public:	
	AInteractableActor();

public:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	bool bPlayerInteractable = true;
};
