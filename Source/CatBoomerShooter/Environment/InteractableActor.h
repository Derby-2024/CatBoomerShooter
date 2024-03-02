// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Character/InteractInterface.h"
#include "InteractableActor.generated.h"

UCLASS()
class CATBOOMERSHOOTER_API AInteractableActor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	bool bPlayerInteractable = true;

	// Interface functions
	bool OnInteract(AActor* OwningActor); virtual bool OnInteract_Implementation(AActor* OwningActor) override;
};
