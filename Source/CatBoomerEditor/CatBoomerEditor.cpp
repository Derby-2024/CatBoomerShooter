// Copyright Epic Games, Inc. All Rights Reserved.

#include "CatBoomerEditor.h"
#include "Modules/ModuleManager.h"

#include <UnrealEdGlobals.h>
#include <Editor/UnrealEdEngine.h>

#include "MeshVisualizer.h"

IMPLEMENT_MODULE(FCatBoomerShooterEditorModule, CatBoomerEditor);

void FCatBoomerShooterEditorModule::StartupModule()
{
	if (GUnrealEd) {
		TSharedPtr<FMeshVisualizer> Visualizer = MakeShared<FMeshVisualizer>();
		GUnrealEd->RegisterComponentVisualizer(UStaticMeshComponent::StaticClass()->GetFName(), Visualizer);
	}
}

void FCatBoomerShooterEditorModule::ShutdownModule()
{
	if (GUnrealEd) {
		GUnrealEd->UnregisterComponentVisualizer(UStaticMeshComponent::StaticClass()->GetFName());
	}
}


