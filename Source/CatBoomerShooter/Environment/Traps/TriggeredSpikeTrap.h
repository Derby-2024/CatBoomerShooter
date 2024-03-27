// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "TriggeredSpikeTrap.generated.h"

UCLASS()
class CATBOOMERSHOOTER_API ATriggeredSpikeTrap : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ATriggeredSpikeTrap();

	UPROPERTY(EditAnywhere, Category = "Traps")
	bool Act = false;

	FTimerHandle ActTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Traps")
	float TimeToActivate = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Traps")
	float TimeToDeactivate = 2.0f;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//static mesh for the spike
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* StaticMesh;

	//static mesh for the pressure plate
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* StaticMesh2;

	//box collision with the spike
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxCollider;

	// box collision with the pressure plate
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxCollider2;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//this casues the spike to move up so the player can get damage
	void Activate();

	//this causes the spike to move downwards so the player cant be damaged by it
	void Deactivate();

	//overlaps with the spike, casues player damage
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//overlap with pressure plate, this activates the spike to shoot up
	UFUNCTION()
	void OnOverlap2(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
