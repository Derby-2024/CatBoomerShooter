// Fill out your copyright notice in the Description page of Project Settings.


#include "RotatingTrap.h"

// Sets default values
ARotatingTrap::ARotatingTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	BoxCollider->SetupAttachment(StaticMesh);

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ARotatingTrap::OnOverlap);
}

// Called when the game starts or when spawned
void ARotatingTrap::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARotatingTrap::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this) {
		UE_LOG(LogTemp, Warning, TEXT("Damage from spinning trap"));
	}
}


// Called every frame
void ARotatingTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//using a fquat instead of a frotator as the actor can randomly get stuck
	FQuat QuatRotation = FQuat(FRotator(0, SpinSpeed, 0));

	AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);

}

