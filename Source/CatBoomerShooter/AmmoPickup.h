// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickup.h"
#include "Character/BasePlayerCharacter.h"
#include "BaseAmmo.h"
#include "Components/SphereComponent.h"
#include "AmmoPickup.generated.h"

/**
 * 
 */
UCLASS()
class CATBOOMERSHOOTER_API AAmmoPickup : public ABasePickup
{
	GENERATED_BODY()
	
public:
    AAmmoPickup();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo Pickup")
    int32 Quantity;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo Pickup")
    EAmmoType AmmoType;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    USphereComponent* CollisionComponent;

public:
    virtual void OnPickup() override;

    UFUNCTION()
    void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
