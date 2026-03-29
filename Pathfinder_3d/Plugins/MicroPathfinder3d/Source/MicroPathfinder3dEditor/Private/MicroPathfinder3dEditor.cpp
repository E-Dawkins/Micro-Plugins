// Copyright Epic Games, Inc. All Rights Reserved.

#include "MicroPathfinder3dEditor.h"

#if WITH_EDITOR
#include "PFVolumeDebugVisualizer.h"
#include "PFVolumeDebugComponent.h"
#include "UnrealEd.h"
#include "NodeStyleCustomization.h"
#endif

#define LOCTEXT_NAMESPACE "FMicroPathfinder3dEditorModule"

// Below we *don't* wrap Startup/Shutdown module functions as they are not editor-only
// All other functions in this module are editor-only, and should be treated as such

void FMicroPathfinder3dEditorModule::StartupModule()
{
#if WITH_EDITOR
    // Since the module is loading phase 'Editor' we can immediately register property stuff...
    RegisterSectionFilters();
    RegisterPropertyCustomizations();

    // ...but need to defer visualizer registration for when the engine has fully initialized
    if (GUnrealEd)
    {
        RegisterVisualizers();
    }
    else
    {
        FCoreDelegates::OnPostEngineInit.AddRaw(this, &FMicroPathfinder3dEditorModule::RegisterVisualizers);
    }
#endif
}

void FMicroPathfinder3dEditorModule::ShutdownModule()
{
#if WITH_EDITOR
    UnregisterVisualizers();
    UnregisterPropertyCustomizations();
    UnregisterSectionFilters();
#endif
}

#if WITH_EDITOR
void FMicroPathfinder3dEditorModule::RegisterSectionFilters()
{
    FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

    TSharedRef<FPropertySection> Section = PropertyModule.FindOrCreateSection("PFVolume", "PathfindVolume", FText::FromString("Pathfind Volume"));
    Section->AddCategory("PF Volume");
    Section->AddCategory("PF Volume - Debug");
}

void FMicroPathfinder3dEditorModule::RegisterPropertyCustomizations()
{
    FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

    PropertyModule.RegisterCustomPropertyTypeLayout(
        "NodeStyle",
        FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FNodeStyleCustomization::MakeInstance)
    );

    PropertyModule.RegisterCustomPropertyTypeLayout(
        "NodeStyleArray",
        FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FNodeStyleArrayCustomization::MakeInstance)
    );
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

void FMicroPathfinder3dEditorModule::UnregisterPropertyCustomizations()
{
    if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
    {
        FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

        PropertyModule.UnregisterCustomPropertyTypeLayout("NodeStyle");
        PropertyModule.UnregisterCustomPropertyTypeLayout("NodeStyleArray");
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
#endif

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMicroPathfinder3dEditorModule, MicroPathfinder3dEditor)