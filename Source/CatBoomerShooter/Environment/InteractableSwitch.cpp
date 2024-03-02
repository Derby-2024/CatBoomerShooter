// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableSwitch.h"

void AInteractableSwitch::BeginPlay()
{
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
				IInteractInterface* Interface = Cast<IInteractInterface>(LinkedActor);
				Interface->Execute_OnInteract(LinkedActor, this);
			}
		}
		return true;
	}

	return false;
}
