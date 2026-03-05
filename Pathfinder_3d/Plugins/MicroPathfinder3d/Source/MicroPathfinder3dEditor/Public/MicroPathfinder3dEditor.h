// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FMicroPathfinder3dEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void RegisterSectionFilters();
	void RegisterVisualizers();

	void UnregisterSectionFilters();
	void UnregisterVisualizers();

private:
    TSharedPtr<class FPFVolumeDebugVisualizer> Visualizer;
};