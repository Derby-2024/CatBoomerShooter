// Fill out your copyright notice in the Description page of Project Settings.


#include "SecretHintAI.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SplineComponent.h"
#include "AIController.h"



/* Spawns, remains stationary until player comes within a certain distance of it, when player comes into distance it will make progress across a predetermined path
nodes will be created along this path and will stop at until the player comes within distance of the next point.  
Also check if it wants to respawn after the secret has been collected in the past for replayability. 
*/ 

// Sets default values
ASecretHintAI::ASecretHintAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASecretHintAI::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASecretHintAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector playerPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector hintPos = GetActorLocation();

	FVector::Distance(playerPos, hintPos);

	/*USplineComponent* Spline = CCatPath();
	CCatPath->GetSplineComponent();
	
	// Calls Move to for the amount of spline points that are in the path. 
	for (int SplineCount = 0; SplineCount < (Spline->GetNumberOfSplinePoints() - 1); SplineCount++) AAIController::MoveTo(GetLocationAtSplinePoint(SplineCount));
	
	while (FVector::Distance(playerPos, hintPos) <= activeMoveDist)
	{
		hintMoveSpeed = 5.0f; 
		
		 if (y == nodearray.getlength + 1)
		{
			GetOwner->Destroy();
		}   
	}*/

	//while(FVector::Distance(playerPos, hintPos) != activeMoveDist)
		//hintMoveSpeed = 0.f; 
}

// Called to bind functionality to input
void ASecretHintAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

