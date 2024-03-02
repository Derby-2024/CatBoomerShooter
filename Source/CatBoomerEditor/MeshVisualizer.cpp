// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshVisualizer.h"
#include "CatBoomerShooter/Environment/InteractableDoor.h"

void FMeshVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	const UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(Component);
	if (!ensure(Mesh)) {
		return;
	}

	// If Door, draw custom preview wirebox
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
}
