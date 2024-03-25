// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolVisualizer.h"

#include "Kismet/KismetMathLibrary.h"

#include "CatBoomerShooter/AI/Patrol/PatrolPoint.h"

void FPatrolVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	const UPatrolComponent* Patrol = Cast<UPatrolComponent>(Component);
	if (!Patrol) return;

	if (Patrol->PatrolPoints.Num() <= 1) return;

	for (int x = 0; x < Patrol->PatrolPoints.Num() - 1; x++)
	{
		const FPatrolData PointData = Patrol->PatrolPoints[x];

		if (!PointData.PatrolPoint) continue;

		DrawPoint(PointData, PDI);

		// Get next valid point
		const APatrolPoint* NextPoint = nullptr;
		while (x < Patrol->PatrolPoints.Num() - 1)
		{
			NextPoint = Patrol->PatrolPoints[x + 1].PatrolPoint;
			if (NextPoint) break;

			x++;
		}
		if (!NextPoint) continue;

		DrawConnection(PointData.PatrolPoint, NextPoint, PDI);
	}

	const APatrolPoint* FirstPoint = Patrol->PatrolPoints[0].PatrolPoint;
	const FPatrolData LastPointData = Patrol->PatrolPoints.Last();
	const APatrolPoint* LastPoint = LastPointData.PatrolPoint;

	if (LastPoint) 
	{
		DrawPoint(LastPointData, PDI);
		if (FirstPoint) DrawConnection(LastPoint, FirstPoint, PDI);
	}
}

void FPatrolVisualizer::DrawPoint(const FPatrolData Point, FPrimitiveDrawInterface* PDI)
{
	FVector StartPos = Point.PatrolPoint->GetActorLocation() + FVector(0., 0., 25.);
	//FVector ForwardVector = Point.PatrolPoint->GetActorForwardVector();
	FRotator Rotation = Point.PatrolPoint->GetActorRotation();
	float Angle = Rotation.Euler().Z;
	

	DrawWireCylinder(
		PDI,
		StartPos,
		FVector::UnitX(),
		FVector::UnitY(),
		FVector::UnitZ(),
		FColorList::Red,
		Point.PatrolPoint->PatrolPointRange,
		25.,	// Height
		5,		// Num sides
		100		// Depth Priority
	);

	if (Point.UsePatrolPointRotation)
	{
		DrawArc(
			PDI,
			StartPos, 
			FVector::UnitX(),
			FVector::UnitY(),
			Angle - Point.RotationVariance,
			Angle + Point.RotationVariance,
			Point.PatrolPoint->PatrolPointRange * 0.8,
			4,
			FColorList::Blue,
			100
		);
	}
}

void FPatrolVisualizer::DrawConnection(const APatrolPoint* StartPoint, const APatrolPoint* EndPoint, FPrimitiveDrawInterface* PDI)
{
	FVector Start = StartPoint->GetActorLocation();
	FVector End = EndPoint->GetActorLocation();

	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Start, End);
	float Distance = FVector::Distance(Start, End) - EndPoint->PatrolPointRange;

	FTransform Transform = FTransform(Rotation, Start + FVector(0., 0., 25.), FVector::One());

	Transform.ToMatrixNoScale();

	DrawDirectionalArrow(
		PDI, 
		Transform.ToMatrixNoScale(), 
		FColorList::Orange, 
		Distance, 
		5.f,	// Arrow Size
		100,	// Depth Priority
		5.f		// Thickness
	);
}
