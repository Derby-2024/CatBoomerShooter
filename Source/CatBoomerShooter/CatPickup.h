// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickup.h"
#include "InventoryComponent.h"
#include "Components/SphereComponent.h"
#include "CatPickup.generated.h"

/**
 * 
 */
UCLASS()
class CATBOOMERSHOOTER_API ACatPickup : public ABasePickup
{
	GENERATED_BODY()

public:
    ACatPickup();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CatPickup")
    FString ItemInformation;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CatPickup")
    int32 Quantity;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CatPickup")
    ECollectibleType CollectibleType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
    USphereComponent* CollisionComponent;

public:
    // Override the OnPickup function to handle picking up the key
    virtual void OnPickup() override;

    UFUNCTION()
    void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    void RunawaySequence(AActor* BasePlayerCharacter);
};
