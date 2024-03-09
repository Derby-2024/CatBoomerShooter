// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableDoorMesh.h"
#include "InteractableDoor.h"

void UInteractableDoorMesh::BeginPlay()
{
	InitialRotation = GetComponentQuat();
	InitialLocation = GetComponentLocation();
}

const FQuat UInteractableDoorMesh::GetTargetRotation(EDoorState TargetState)
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

const FVector UInteractableDoorMesh::GetTargetLocation(EDoorState TargetState)
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