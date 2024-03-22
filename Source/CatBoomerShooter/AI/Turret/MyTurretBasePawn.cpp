// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTurretBasePawn.h"
//
AMyTurretBasePawn::AMyTurretBasePawn() : Super()
{
	EyeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EyeMesh"));
	EyeMesh->SetupAttachment(RootComponent);
} 

void AMyTurretBasePawn::GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const
{
	Location = EyeMesh->GetComponentLocation();
	Rotation = EyeMesh->GetComponentRotation();
}