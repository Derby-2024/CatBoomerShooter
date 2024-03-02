// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshVisualizer.h"
#include "CatBoomerShooter/Environment/InteractableDoor.h"
#include "CatBoomerShooter/Environment/InteractableSwitch.h"

void FMeshVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	const UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(Component);
	if (!ensure(Mesh)) {
		return;
	}

	// Interactable Door Mesh Visualizer
	if (const AInteractableDoor* DoorActor = Cast<AInteractableDoor>(Mesh->GetOwner())) {
		const FBox MeshBounds = DoorActor->CalculateComponentsBoundingBoxInLocalSpace();

		FTransform ActorToWorld = DoorActor->ActorToWorld();

		switch (DoorActor->OpenMethod) {
		case EDoorOpenMethod::Rotate:
			ActorToWorld.ConcatenateRotation(FQuat::MakeFromEuler(DoorActor->OpenDelta));
			break;
		case EDoorOpenMethod::Move:
			ActorToWorld.AddToTranslation(DoorActor->OpenDelta);
			break;

		default:
			checkNoEntry();
		}

		const FMatrix BoxToWorldMatrix = ActorToWorld.ToMatrixNoScale();

		DrawWireBox(PDI, BoxToWorldMatrix, MeshBounds, FColorList::Red, 100);
	}

	// Interactable Switch linked visualizer
	else if (const AInteractableSwitch* SwitchActor = Cast<AInteractableSwitch>(Mesh->GetOwner())) {
		const FVector StartLocation = SwitchActor->GetActorLocation();

		for (AActor* LinkedActor : SwitchActor->LinkedActors) {
			if(LinkedActor)
				DrawDashedLine(PDI, StartLocation, LinkedActor->GetActorLocation(), FColorList::Red, 1, 100);
		}
	}
}
