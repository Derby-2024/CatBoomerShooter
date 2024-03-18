// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	class UStaticMeshComponent* staticMesh;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Activate();

	void Deactivate();
};
