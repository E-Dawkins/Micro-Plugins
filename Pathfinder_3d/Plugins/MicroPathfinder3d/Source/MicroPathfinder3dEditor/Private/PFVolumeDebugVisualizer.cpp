#include "PFVolumeDebugVisualizer.h"

#include "PFVolumeDebugComponent.h"
#include "PFVolume.h"

#include "SceneManagement.h"

void FPFVolumeDebugVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
    const UPFVolumeDebugComponent* DebugComp = Cast<UPFVolumeDebugComponent>(Component);
    if (!IsValid(DebugComp) || !DebugComp->bVisualizePoints) return;

    const APFVolume* Owner = Cast<APFVolume>(DebugComp->GetOwner());
    if (!IsValid(Owner)) return;

    const FVector OffsetOwnerLocation = Owner->GetActorLocation() - Owner->GetBounds().BoxExtent;

    // Visualize all node points
	for (const auto& [AxisIndices, Value] : Owner->Nodes)
	{
        if (Value == ENodeType::None) continue; // Failsafe

        // Offset by -1 to account for 'None' being 0
        FLinearColor PointColor = DebugComp->NodeStyles.Items[(uint8)Value - 1].Color;

        if (PointColor.A == 0.f) continue; // Skip 'clear' colors

        PDI->DrawPoint(OffsetOwnerLocation + (FVector(AxisIndices) * Owner->CellSize), PointColor, 5.f, SDPG_World);
	}
}
