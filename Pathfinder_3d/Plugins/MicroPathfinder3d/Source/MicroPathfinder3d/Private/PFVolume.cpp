// Fill out your copyright notice in the Description page of Project Settings.


#include "PFVolume.h"

#include "PFVolumeDebugComponent.h"

APFVolume::APFVolume()
{
    UPFVolumeDebugComponent* DebugComp = CreateDefaultSubobject<UPFVolumeDebugComponent>(TEXT("DebugComponent"));
}

void APFVolume::PopulateGrid()
{
	Cells.Empty();

	const FVector HalfExtent = GetBounds().BoxExtent;
	const FIntVector MaxCellCount = FIntVector((HalfExtent * 2.f) / CellSize);
	const FVector StartPos = GetActorLocation() - HalfExtent;

	for (int32 X = 0; X <= MaxCellCount.X; X++)
	{
		for (int32 Y = 0; Y <= MaxCellCount.Y; Y++)
		{
			for (int32 Z = 0; Z <= MaxCellCount.Z; Z++)
			{
				Cells.Push(FGridCell{
					.WorldPosition = StartPos + FVector(X, Y, Z) * CellSize
				});
			}
		}
	}
}

#if WITH_EDITOR
void APFVolume::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	if (bFinished)
	{
		if (bIsUpdatingPoints)
			return;

		TGuardValue<bool> Guard(bIsUpdatingPoints, true);

		PopulateGrid();
	}
}
#endif
