// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTurretBaseCharacter.h"

void AMyTurretBaseCharacter::GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const
{
	Location = GetMesh()->GetComponentLocation();
	Rotation = GetMesh()->GetComponentRotation();
}
