// Copyright Epic Games, Inc. All Rights Reserved.

#include "MicroPathfinder3dEditor.h"

#include "PFVolumeDebugVisualizer.h"
#include "PFVolumeDebugComponent.h"
#include "UnrealEd.h"

#define LOCTEXT_NAMESPACE "FMicroPathfinder3dEditorModule"

void FMicroPathfinder3dEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
    Visualizer = MakeShareable(new FPFVolumeDebugVisualizer);

    if (GUnrealEd)
    {
        GUnrealEd->RegisterComponentVisualizer(
            UPFVolumeDebugComponent::StaticClass()->GetFName(),
            Visualizer
        );
        Visualizer->OnRegister();
    }
}

void FMicroPathfinder3dEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
    if (GUnrealEd)
    {
        GUnrealEd->UnregisterComponentVisualizer(
            UPFVolumeDebugComponent::StaticClass()->GetFName()
        );
    }
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMicroPathfinder3dEditorModule, MicroPathfinder3dEditor)