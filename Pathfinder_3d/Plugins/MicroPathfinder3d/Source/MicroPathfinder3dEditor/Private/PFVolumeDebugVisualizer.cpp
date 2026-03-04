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

    const FVector Center = Owner->GetActorLocation();
    const FVector HalfExtent = FVector(50.f);

    const FBox Box = FBox(Center - HalfExtent, Center + HalfExtent);

    DrawWireBox(PDI, Box, FLinearColor::Yellow, SDPG_Foreground, 2.f);
}
