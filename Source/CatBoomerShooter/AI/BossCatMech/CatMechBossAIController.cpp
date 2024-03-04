// Fill out your copyright notice in the Description page of Project Settings.


#include "CatMechBossAIController.h"
#include "Kismet/KismetMathLibrary.h"

// https://dev.epicgames.com/community/learning/tutorials/ZYVB/unreal-engine-smooth-focus-using-ai-setfocus-setfocalpoint

void ACatMechBossAIController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
	//Call the original method without updating the pawn
	Super::UpdateControlRotation(DeltaTime, false);

	//Smooth and change the pawn rotation
	if (bUpdatePawn)
	{
		//Get pawn
		APawn* const MyPawn = GetPawn();
		//Get Pawn current rotation
		const FRotator CurrentPawnRotation = MyPawn->GetActorRotation();

		//Calculate smoothed rotation
		SmoothTargetRotation = UKismetMathLibrary::RInterpTo_Constant(MyPawn->GetActorRotation(), ControlRotation, DeltaTime, SmoothFocusInterpSpeed);
		//Check if we need to change
		if (CurrentPawnRotation.Equals(SmoothTargetRotation, 1e-3f) == false)
		{
			//Change rotation using the Smooth Target Rotation
			MyPawn->FaceRotation(SmoothTargetRotation, DeltaTime);
		}
	}
}