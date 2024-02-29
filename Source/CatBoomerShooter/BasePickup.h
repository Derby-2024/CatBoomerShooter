// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePickup.generated.h"

UCLASS()
class CATBOOMERSHOOTER_API ABasePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasePickup();

protected:
    // Item info
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
    FString ItemInfo;

    // Static mesh component for the pickup
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
    class UStaticMeshComponent* PickupMesh;

public:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called when this pickup is picked up
    virtual void OnPickup();

};
