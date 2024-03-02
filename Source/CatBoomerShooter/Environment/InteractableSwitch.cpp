// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableSwitch.h"

void AInteractableSwitch::BeginPlay()
{
	// Disable player interaction for any linked actors
	for (AActor* LinkedActor : LinkedActors) {
		Cast<AInteractableActor>(LinkedActor)->bPlayerInteractable = false;
	}
}

bool AInteractableSwitch::OnInteract_Implementation(AActor* OwningActor)
{
	if (LinkedActors.Num() > 0) {
		for (AActor* LinkedActor : LinkedActors)
		{
			if (LinkedActor) {
				IInteract* Interface = Cast<IInteract>(LinkedActor);
				Interface->Execute_OnInteract(LinkedActor, this);
			}
		}
		return true;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("InteractableSwitch OnInteract: Linked Actors Array is empty"));
		return false;
	}
}
