// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Director/AIDirectorGameMode.h"

// Sets default values
AEnemyBaseCharacter::AEnemyBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add("Enemy");
}

// Called when the game starts or when spawned
void AEnemyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	AAIDirectorGameMode* AIDirector = 
		Cast<AAIDirectorGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!AIDirector)
	{
		UE_LOG(LogTemp, Warning, TEXT("AEnemyBaseCharacter::BeginPlay: Could not get AIDirector Game mode."));
		return;
	}

	AIDirector->RegisterEnemy(this);
	
}

void AEnemyBaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (EndPlayReason == EEndPlayReason::Destroyed || EndPlayReason == EEndPlayReason::RemovedFromWorld)
	{
		AAIDirectorGameMode* AIDirector =
			Cast<AAIDirectorGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		if (AIDirector) AIDirector->RemoveEnemy(this);
	}

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AEnemyBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

EEnemySize AEnemyBaseCharacter::GetEnemySize_Implementation()
{
	return EnemySize;
}