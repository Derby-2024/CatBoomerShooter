// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EMSActorSaveInterface.h"
#include "BasePickup.generated.h"

UCLASS()
class CATBOOMERSHOOTER_API ABasePickup : public AActor, public IEMSActorSaveInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasePickup();

protected:
    // Item info
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
    FString ItemInfo;

    UPROPERTY(SaveGame)
    bool bIsDisabled = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    class USphereComponent* CollisionComponent;

    // Static mesh component for the pickup
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
    class UStaticMeshComponent* PickupMesh;

public:
    UPROPERTY(EditAnywhere)
    float PickupRange = 50.0f;

public:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called when this pickup is picked up
    virtual void OnPickup();

    UFUNCTION()
    virtual void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    void CheckActorDisabled();

    // EMS Interface
    void ActorLoaded_Implementation() override;
};
