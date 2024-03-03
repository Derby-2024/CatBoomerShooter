// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickup.h"
#include "InventoryComponent.h"
#include "Components/SphereComponent.h"
#include "KeyPickup.generated.h"


/**
 * 
 */
UCLASS()
class CATBOOMERSHOOTER_API AKeyPickup : public ABasePickup
{
	GENERATED_BODY()
public:
    AKeyPickup();

protected:
    // Unique string variable for the key pickup
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key")
    FString ItemInformation;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
    USphereComponent* CollisionComponent;

public:
    // Override the OnPickup function to handle picking up the key
    virtual void OnPickup() override;

    UFUNCTION()
    void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
