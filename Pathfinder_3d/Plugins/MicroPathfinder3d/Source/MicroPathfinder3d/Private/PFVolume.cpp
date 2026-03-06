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

	FVector Origin, BoxExtent;
	GetActorBounds(false, Origin, BoxExtent);

	const FVector Min = (Origin - BoxExtent);
	const FVector Max = (Origin + BoxExtent);

	for (int32 X = Min.X; X <= Max.X; X += CellSize.X)
	{
		for (int32 Y = Min.Y; Y <= Max.Y; Y += CellSize.Y)
		{
			for (int32 Z = Min.Z; Z <= Max.Z; Z += CellSize.Z)
			{
				const FVector Point = FVector(X, Y, Z);

				if (EncompassesPoint(Point, KINDA_SMALL_NUMBER))
				{
					Cells.Push(FGridCell{
						.WorldPosition = Point
					});
				}
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
