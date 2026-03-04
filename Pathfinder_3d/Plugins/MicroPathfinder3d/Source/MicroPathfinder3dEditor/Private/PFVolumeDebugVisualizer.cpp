#include "PFVolumeDebugVisualizer.h"

#include "PFVolumeDebugComponent.h"
#include "PFVolume.h"

#include "SceneManagement.h"

void FPFVolumeDebugVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
    const UPFVolumeDebugComponent* DebugComp = Cast<UPFVolumeDebugComponent>(Component);
    if (!IsValid(DebugComp)) return;

    const APFVolume* Owner = Cast<APFVolume>(DebugComp->GetOwner());
    if (!IsValid(Owner)) return;

    for (FGridCell Cell : Owner->Cells)
    {
        PDI->DrawPoint(Cell.WorldPosition, FColor::Cyan, 5.f, SDPG_World);
    }
}
