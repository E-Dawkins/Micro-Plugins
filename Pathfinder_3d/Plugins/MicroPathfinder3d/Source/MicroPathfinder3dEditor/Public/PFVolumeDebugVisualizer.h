#pragma once

#include "ComponentVisualizer.h"

class FPFVolumeDebugVisualizer : public FComponentVisualizer
{
public:
    virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;

};