// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SecretHint_Ai.h"
#include "Kismet/GameplayStatics.h"
#include "Director/AIDirectorGameMode.h"

// Sets default values
ASecretHint_Ai::ASecretHint_Ai()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASecretHint_Ai::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASecretHint_Ai::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASecretHint_Ai::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

