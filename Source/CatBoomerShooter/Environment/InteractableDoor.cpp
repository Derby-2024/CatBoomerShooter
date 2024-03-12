// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableDoor.h"
#include "InteractableDoorMesh.h"
#include "../Character/BasePlayerCharacter.h"


AInteractableDoor::AInteractableDoor() {
	Mesh->DestroyComponent();
}

void AInteractableDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialRotation = GetActorQuat();
	InitialLocation = GetActorLocation();
}

void AInteractableDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UInteractableDoorMesh*> DoorMeshArr;
	GetComponents<UInteractableDoorMesh>(DoorMeshArr);

	for (UInteractableDoorMesh* DoorMesh : DoorMeshArr) {
		switch (OpenMethod) {
		case EDoorOpenMethod::Rotate: {
			const FQuat CurrentRotation = DoorMesh->GetComponentQuat();
			const FQuat TargetRotation = DoorMesh->GetTargetRotation(TargetState);

			FQuat NewRotation;

			if (CurrentRotation.Equals(TargetRotation))
				NewRotation = TargetRotation;
			else
				NewRotation = FQuat::Slerp(CurrentRotation, TargetRotation, OpenSpeed * DeltaTime);
			
			DoorMesh->SetWorldRotation(NewRotation);
			break;
		}
		case EDoorOpenMethod::Move: {
			const FVector CurrentLocation = DoorMesh->GetComponentLocation();
			const FVector TargetLocation = DoorMesh->GetTargetLocation(TargetState);

			FVector NewLocation;

			if (CurrentLocation.Equals(TargetLocation))
				NewLocation = TargetLocation;
			else
				NewLocation = FMath::Lerp(CurrentLocation, TargetLocation, OpenSpeed * DeltaTime);

			DoorMesh->SetWorldLocation(NewLocation);
			break;
		}

		default:
			checkNoEntry();
		}
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
		if (!OwningPlayer->Keys.Contains(RequiredKey))
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
