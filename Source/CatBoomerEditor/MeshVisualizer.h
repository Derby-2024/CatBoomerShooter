// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <ComponentVisualizer.h>

class FMeshVisualizer : public FComponentVisualizer
{

private:
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
};
