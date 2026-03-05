// Copyright Epic Games, Inc. All Rights Reserved.

#include "MicroPathfinder3dEditor.h"

#include "PFVolumeDebugVisualizer.h"
#include "PFVolumeDebugComponent.h"
#include "UnrealEd.h"

#define LOCTEXT_NAMESPACE "FMicroPathfinder3dEditorModule"

void FMicroPathfinder3dEditorModule::StartupModule()
{
    // Since the module is loading phase 'Editor' we can immediately register section filters...
    RegisterSectionFilters();

    // ...but need to defer visualizer registration for when the engine has fully initialized
    if (GUnrealEd)
    {
        RegisterVisualizers();
    }
    else
    {
        FCoreDelegates::OnPostEngineInit.AddRaw(this, &FMicroPathfinder3dEditorModule::RegisterVisualizers);
    }
}

void FMicroPathfinder3dEditorModule::ShutdownModule()
{
    UnregisterVisualizers();
    UnregisterSectionFilters();
}

void FMicroPathfinder3dEditorModule::RegisterSectionFilters()
{
#if WITH_EDITOR
    FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

    TSharedRef<FPropertySection> Section = PropertyModule.FindOrCreateSection("PFVolume", "PathfindVolume", FText::FromString("Pathfind Volume"));
    Section->AddCategory("PF Volume");
    Section->AddCategory("PF Volume - Debug");
#endif
}

void FMicroPathfinder3dEditorModule::RegisterVisualizers()
{
    if (GUnrealEd)
    {
        Visualizer = MakeShareable(new FPFVolumeDebugVisualizer);
        GUnrealEd->RegisterComponentVisualizer(UPFVolumeDebugComponent::StaticClass()->GetFName(), Visualizer);
        Visualizer->OnRegister();
    }
}

void FMicroPathfinder3dEditorModule::UnregisterSectionFilters()
{
    if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
    {
        FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

        PropertyModule.RemoveSection("PFVolume", "PathfindVolume");
    }
}

void FMicroPathfinder3dEditorModule::UnregisterVisualizers()
{
    if (GUnrealEd)
    {
        if (Visualizer.IsValid())
        {
            GUnrealEd->UnregisterComponentVisualizer("PFVolumeDebugComponent");
            Visualizer.Reset();
        }
    }
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMicroPathfinder3dEditorModule, MicroPathfinder3dEditor)