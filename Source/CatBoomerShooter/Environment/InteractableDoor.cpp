// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableDoor.h"
#include "../Character/BasePlayerCharacter.h"

void AInteractableDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialRotation = GetActorQuat();
	InitialLocation = GetActorLocation();
}

FQuat AInteractableDoor::GetTargetRotation() const
{
	switch (TargetState) {
	case EDoorState::Closed:
		return InitialRotation;
	case EDoorState::Opened:
		return InitialRotation * FQuat::MakeFromEuler(OpenDelta);

	default:
		checkNoEntry();
		return {};
	}
}

FVector AInteractableDoor::GetTargetLocation() const
{
	switch (TargetState) {
	case EDoorState::Closed:
		return InitialLocation;
	case EDoorState::Opened:
		return InitialLocation + OpenDelta;

	default:
		checkNoEntry();
		return{};
	}
}

void AInteractableDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Smooth interpolate transform
	// TODO if needed: Convert logic to utlize a transform rather than splitting into 2 seperate logic
	switch (OpenMethod) {
	case EDoorOpenMethod::Rotate: {
		const FQuat CurrentRotation = GetActorQuat();
		const FQuat TargetRotation = GetTargetRotation();

		FQuat NewRotation;

		if (CurrentRotation.Equals(TargetRotation))
			NewRotation = TargetRotation;
		else
			NewRotation = FQuat::Slerp(CurrentRotation, TargetRotation, OpenSpeed * DeltaTime);

		SetActorRotation(NewRotation);
	}
		break;
	case EDoorOpenMethod::Move: {
		const FVector CurrentLocation = GetActorLocation();
		const FVector TargetLocation = GetTargetLocation();

		FVector NewLocation;

		if (CurrentLocation.Equals(TargetLocation))
			NewLocation = TargetLocation;
		else
			NewLocation = FMath::Lerp(CurrentLocation, TargetLocation, OpenSpeed * DeltaTime);

		SetActorLocation(NewLocation);
	}
		break;

	default:
		checkNoEntry();
	}
}

bool AInteractableDoor::OnInteract_Implementation(AActor* OwningActor)
{
	// Return false if not interactable by player
	if (!bPlayerInteractable && Cast<ABasePlayerCharacter>(OwningActor)) {
		return false;
	}

	// Check for key
	ABasePlayerCharacter* OwningPlayer = Cast<ABasePlayerCharacter>(OwningActor);
	if (bRequireKey && OwningPlayer) {
		if (!OwningPlayer->GetInventoryComponent()->Keys.Contains(RequiredKey))
			return false;
	}

	// toggle state
	switch(TargetState) {
		case EDoorState::Closed:
			TargetState = EDoorState::Opened;
			return true;
		case EDoorState::Opened:
			if (bCanBeClosed) {
				TargetState = EDoorState::Closed;
				return true;
			}
			return false;

		default:
			checkNoEntry();
			return false;
	}
}
