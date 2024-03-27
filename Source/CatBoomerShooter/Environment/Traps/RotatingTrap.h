// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "RotatingTrap.generated.h"

UCLASS()
class CATBOOMERSHOOTER_API ARotatingTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotatingTrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Traps")
	float SpinSpeed = 0.0f;

	//static mesh and collision boxes
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxCollider;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//called when the player overlaps with the collision box, causes damage to player
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
