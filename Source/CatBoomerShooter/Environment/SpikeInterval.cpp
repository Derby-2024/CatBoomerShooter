// Fill out your copyright notice in the Description page of Project Settings.


#include "SpikeInterval.h"

// Sets default values
ASpikeInterval::ASpikeInterval()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
}

// Called when the game starts or when spawned
void ASpikeInterval::BeginPlay()
{
	Super::BeginPlay();
	if (Act != true) {
		GetWorld()->GetTimerManager().SetTimer(ActTimerHandle, this, &ASpikeInterval::Activate, 5.0f, false);
	}
}

void ASpikeInterval::Activate() {
	FVector Location = GetActorLocation();
	Location.Z += 100;
	SetActorLocation(Location);
	GetWorld()->GetTimerManager().SetTimer(ActTimerHandle, this, &ASpikeInterval::Deactivate, 2.5f, false);

}
void ASpikeInterval::Deactivate()
{
	FVector Location = GetActorLocation();
	Location.Z -= 100;
	SetActorLocation(Location);
	Act = true;
	GetWorld()->GetTimerManager().SetTimer(ActTimerHandle, this, &ASpikeInterval::Activate, 5.0f, false);

}

// Called every frame
void ASpikeInterval::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

