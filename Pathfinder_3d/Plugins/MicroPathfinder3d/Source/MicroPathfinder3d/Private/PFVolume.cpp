// Fill out your copyright notice in the Description page of Project Settings.


#include "PFVolume.h"

#include "PFVolumeDebugComponent.h"

APFVolume::APFVolume()
{
    UPFVolumeDebugComponent* DebugComp = CreateDefaultSubobject<UPFVolumeDebugComponent>(TEXT("DebugComponent"));
}

const FGridCell& APFVolume::GetNearestGridCell(const FVector& Point)
{
	const int32 Index = GetNearestCellIndex(Point);

	return Cells[Index];
}

const int32 APFVolume::GetNearestCellIndex(const FVector& Point) const
{
	// Get point relative to volume location
	FVector PointLocal = Point - GetActorLocation();
	
	// Shift point to be relative to grid min corner
	const FVector BoxExtent = GetBounds().BoxExtent;
	PointLocal += BoxExtent;

	// Round relative point to get its index per axis
	FIntVector PointRounded(
		FMath::RoundToInt32(PointLocal.X / CellSize.X),
		FMath::RoundToInt32(PointLocal.Y / CellSize.Y),
		FMath::RoundToInt32(PointLocal.Z / CellSize.Z)
	);

	// Clamp indices to valid range
	PointRounded = {
		FMath::Clamp(PointRounded.X, 0, CellCountsPerAxis.X - 1),
		FMath::Clamp(PointRounded.Y, 0, CellCountsPerAxis.Y - 1),
		FMath::Clamp(PointRounded.Z, 0, CellCountsPerAxis.Z - 1)
	};

	// Do some logic to get from per axis index to flattened index
	return PointRounded.X * (CellCountsPerAxis.Y * CellCountsPerAxis.Z)
		+ (PointRounded.Y * CellCountsPerAxis.Z)
		+ PointRounded.Z;
}

void APFVolume::PopulateGrid()
{
	Cells.Empty();

	const FVector BoxExtent = GetBounds().BoxExtent;
	const FIntVector MaxCellCount = FIntVector((BoxExtent * 2.f) / CellSize);
	const FVector StartPos = GetActorLocation() - BoxExtent;

	for (int32 X = 0; X <= MaxCellCount.X; X++)
	{
		for (int32 Y = 0; Y <= MaxCellCount.Y; Y++)
		{
			for (int32 Z = 0; Z <= MaxCellCount.Z; Z++)
			{
				Cells.Push(FGridCell{
					.WorldPosition = StartPos + (FVector(X, Y, Z) * CellSize)
				});
			}
		}
	}

	CellCountsPerAxis = MaxCellCount + FIntVector(1);
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

	// Fail-safe, if multi selecting objects and rotating
	if (USceneComponent* Root = GetRootComponent())
	{
		Root->SetRelativeRotation(FRotator::ZeroRotator);
	}
}

void APFVolume::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (!PropertyChangedEvent.MemberProperty)
		return;

	// If rotation changed in the details panel, reset it
	if (PropertyChangedEvent.MemberProperty->GetFName() == TEXT("RelativeRotation"))
	{
		if (USceneComponent* Root = GetRootComponent())
		{
			Root->SetRelativeRotation(FRotator::ZeroRotator);
		}
	}
}

void APFVolume::EditorApplyRotation(const FRotator& DeltaRotation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
{
	// Ignore rotation changes via transform gizmo
}
#endif
