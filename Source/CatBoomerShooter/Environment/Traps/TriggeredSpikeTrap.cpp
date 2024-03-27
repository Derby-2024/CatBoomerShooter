// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggeredSpikeTrap.h"

// Sets default values
ATriggeredSpikeTrap::ATriggeredSpikeTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	BoxCollider->SetupAttachment(StaticMesh);

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ATriggeredSpikeTrap::OnOverlap);


	StaticMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh 2"));
	BoxCollider2 = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider 2"));
	BoxCollider2->SetupAttachment(StaticMesh2);

	BoxCollider2->OnComponentBeginOverlap.AddDynamic(this, &ATriggeredSpikeTrap::OnOverlap2);

}

// Called when the game starts or when spawned
void ATriggeredSpikeTrap::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATriggeredSpikeTrap::Activate() {
	FVector Location = GetActorLocation();
	Location.Z += 100;
	SetActorLocation(Location);
	GetWorld()->GetTimerManager().SetTimer(ActTimerHandle, this, &ATriggeredSpikeTrap::Deactivate, TimeToDeactivate, false);

}
void ATriggeredSpikeTrap::Deactivate()
{
	FVector Location = GetActorLocation();
	Location.Z -= 100;
	SetActorLocation(Location);
	Act = false;

}

void ATriggeredSpikeTrap::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this) {
		UE_LOG(LogTemp, Warning, TEXT("Damage from the activated spike"));
	}
}


void ATriggeredSpikeTrap::OnOverlap2(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Act != true) {
		UE_LOG(LogTemp, Warning, TEXT("pressure plate triggered activating the spike"));
		Act = true;
		GetWorld()->GetTimerManager().SetTimer(ActTimerHandle, this, &ATriggeredSpikeTrap::Activate, TimeToActivate, false);
	}
}

// Called every frame
void ATriggeredSpikeTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

