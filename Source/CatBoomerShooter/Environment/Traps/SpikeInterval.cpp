// Fill out your copyright notice in the Description page of Project Settings.


#include "SpikeInterval.h"

// Sets default values
ASpikeInterval::ASpikeInterval()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	BoxCollider->SetupAttachment(StaticMesh);

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ASpikeInterval::OnOverlap);
	
}

// Called when the game starts or when spawned
void ASpikeInterval::BeginPlay()
{
	Super::BeginPlay();
	if (Act != true) {
		GetWorld()->GetTimerManager().SetTimer(ActTimerHandle, this, &ASpikeInterval::Activate, TimeToActivate, false);
	}
}

void ASpikeInterval::Activate() {
	FVector Location = GetActorLocation();
	Location.Z += 100;
	SetActorLocation(Location);
	GetWorld()->GetTimerManager().SetTimer(ActTimerHandle, this, &ASpikeInterval::Deactivate, TimeToDeactivate, false);

}
void ASpikeInterval::Deactivate()
{
	FVector Location = GetActorLocation();
	Location.Z -= 100;
	SetActorLocation(Location);
	Act = true;
	GetWorld()->GetTimerManager().SetTimer(ActTimerHandle, this, &ASpikeInterval::Activate, TimeToActivate, false);

}

void ASpikeInterval::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this) {
		UE_LOG(LogTemp, Warning, TEXT("Damage from interval spike trap"));
	}
}

//UE_LOG(LogTemp, Warning, TEXT("Damage"));


// Called every frame
void ASpikeInterval::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

