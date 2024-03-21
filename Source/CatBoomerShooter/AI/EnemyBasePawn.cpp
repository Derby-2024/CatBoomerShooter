// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyBasePawn.h"
#include "Kismet/GameplayStatics.h"
#include "Director/AIDirectorGameMode.h"

// Sets default values
AEnemyBasePawn::AEnemyBasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add("Enemy");

	PawnMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PawnMesh"));
	SetRootComponent(PawnMesh);
}

// Called when the game starts or when spawned
void AEnemyBasePawn::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		AAIDirectorGameMode* AIDirector =
			Cast<AAIDirectorGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		if (!AIDirector)
		{
			UE_LOG(LogTemp, Warning, TEXT("AEnemyBaseCharacter::BeginPlay: Could not get AIDirector Game mode."));
			return;
		}

		AIDirector->RegisterEnemy(this);

	}
}
//
//void AEnemyBasePawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
//{
//	if (HasAuthority())
//	{
//		if (EndPlayReason == EEndPlayReason::Destroyed || EndPlayReason == EEndPlayReason::RemovedFromWorld)
//		{
//			AAIDirectorGameMode* AIDirector =
//			Cast<AAIDirectorGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
//			if (AIDirector) AIDirector->RemoveEnemy(this);
//		}
//	}
//
//	Super::EndPlay(EndPlayReason);
//}

// Called every frame
void AEnemyBasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyBasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

EEnemySize AEnemyBasePawn::GetEnemySize_Implementation()
{
	return EnemySize;
}

EEnemyType AEnemyBasePawn::GetEnemyType_Implementation()
{
	return EnemyType;
}