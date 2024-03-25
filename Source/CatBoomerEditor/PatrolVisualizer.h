// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <ComponentVisualizer.h>

#include "CatBoomerShooter/AI/Patrol/PatrolComponent.h"

class FPatrolVisualizer : public FComponentVisualizer
{

private:
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;

	void DrawPoint(const FPatrolData Point, FPrimitiveDrawInterface* PDI);

	void DrawConnection(const class APatrolPoint* StartPoint, const class APatrolPoint* EndPoint, FPrimitiveDrawInterface* PDI);
};
