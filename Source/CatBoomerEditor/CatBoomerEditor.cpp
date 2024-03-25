// Copyright Epic Games, Inc. All Rights Reserved.

#include "CatBoomerEditor.h"
#include "Modules/ModuleManager.h"

#include <UnrealEdGlobals.h>
#include <Editor/UnrealEdEngine.h>

#include "MeshVisualizer.h"

#include "PatrolVisualizer.h"
#include "CatBoomerShooter/AI/Patrol/PatrolComponent.h"

IMPLEMENT_MODULE(FCatBoomerShooterEditorModule, CatBoomerEditor);

void FCatBoomerShooterEditorModule::StartupModule()
{
	if (GUnrealEd) {
		TSharedPtr<FMeshVisualizer> Visualizer = MakeShared<FMeshVisualizer>();
		GUnrealEd->RegisterComponentVisualizer(UStaticMeshComponent::StaticClass()->GetFName(), Visualizer);

		TSharedPtr<FPatrolVisualizer> PatrolVisualizer = MakeShared<FPatrolVisualizer>();
		GUnrealEd->RegisterComponentVisualizer(UPatrolComponent::StaticClass()->GetFName(), PatrolVisualizer);
	}
}

void FCatBoomerShooterEditorModule::ShutdownModule()
{
	if (GUnrealEd) {
		GUnrealEd->UnregisterComponentVisualizer(UStaticMeshComponent::StaticClass()->GetFName());
		GUnrealEd->UnregisterComponentVisualizer(UPatrolComponent::StaticClass()->GetFName());
	}
}
