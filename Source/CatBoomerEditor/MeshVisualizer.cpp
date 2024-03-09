// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshVisualizer.h"
#include "CatBoomerShooter/Environment/InteractableDoor.h"
#include "CatBoomerShooter/Environment/InteractableDoorMesh.h"
#include "CatBoomerShooter/Environment/InteractableSwitch.h"

void FMeshVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	// Ensure Static Mesh component is valid
	const UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(Component);
	if (ensure(Mesh)) {

		// Interactable Door Mesh Visualizer
		if (const AInteractableDoor* DoorActor = Cast<AInteractableDoor>(Mesh->GetOwner())) {
			if (const UInteractableDoorMesh* DoorMesh = Cast<UInteractableDoorMesh>(Mesh)) {
				const FBox MeshBounds = DoorMesh->CalcLocalBounds().GetBox();
				FTransform MeshToWorld = DoorMesh->GetComponentTransform();

				switch (DoorActor->OpenMethod) {
				case EDoorOpenMethod::Rotate:
					MeshToWorld.ConcatenateRotation(FQuat::MakeFromEuler(DoorMesh->OpenDelta));
					break;
				case EDoorOpenMethod::Move:
					MeshToWorld.AddToTranslation(DoorMesh->OpenDelta);
					break;

				default:
					checkNoEntry();
				}

				const FMatrix BoxToWorldMatrix = MeshToWorld.ToMatrixNoScale();

				DrawWireBox(PDI, BoxToWorldMatrix, MeshBounds, FColorList::Red, 100);
			}
		}

		// Interactable Switch linked visualizer
		else if (const AInteractableSwitch* SwitchActor = Cast<AInteractableSwitch>(Mesh->GetOwner())) {
			const FVector StartLocation = SwitchActor->GetActorLocation();

			for (AActor* LinkedActor : SwitchActor->LinkedActors) {
				if (LinkedActor)
					DrawDashedLine(PDI, StartLocation, LinkedActor->GetActorLocation(), FColorList::Blue, 1, 100);	
			}
		}
	}
}






