// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "SpikeInterval.generated.h"

UCLASS()
class CATBOOMERSHOOTER_API ASpikeInterval : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpikeInterval();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traps")
	bool Act = false;

	FTimerHandle ActTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traps")
	float TimeToActivate = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traps")
	float TimeToDeactivate = 2.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxCollider;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Activate();

	void Deactivate();

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
