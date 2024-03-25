// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolComponent.h"
#include "PatrolPoint.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UPatrolComponent::UPatrolComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UPatrolComponent::BeginPlay()
{
	// Iterate patrol points and remove invalid/unset ones
	for (int x = PatrolPoints.Num() - 1; x >= 0; x--)
	{
		if (!IsValid(PatrolPoints[x].PatrolPoint))
		{
			PatrolPoints.RemoveAt(x);
			UE_LOG(
				LogTemp, Warning, 
				TEXT("Patrol enemy %s has an invalid patrol point set."), 
				*UKismetSystemLibrary::GetDisplayName(GetOwner())
			);
		}
	}

	CurrentPatrolIndex = FMath::Clamp(CurrentPatrolIndex, 0, PatrolPoints.Num() - 1);

	ActorSpawnPoint = GetOwner()->GetActorLocation();
}

void UPatrolComponent::GetCurrentPatrolPoint(FPatrolData& PatrolData, bool& IsValid)
{
	if (PatrolPoints.IsValidIndex(CurrentPatrolIndex))
	{
		IsValid = true;
		PatrolData = PatrolPoints[CurrentPatrolIndex];
	}
}

void UPatrolComponent::IterPatrolPointIndex()
{
	CurrentPatrolIndex = (CurrentPatrolIndex + 1) % PatrolPoints.Num();
}

void UPatrolComponent::SetPatrolPointIndex(int NewIndex)
{
	if (PatrolPoints.IsValidIndex(NewIndex))
	{
		CurrentPatrolIndex = NewIndex;
	}
}
