// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "PFVolume.generated.h"

USTRUCT(BlueprintType)
struct FGridCell
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector WorldPosition = {};
};

UCLASS(meta = (DisplayName = "PF Volume"))
class MICROPATHFINDER3D_API APFVolume : public AVolume
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "PF Volume")
	FVector CellSize = FVector(100.f);

private:
	UPROPERTY(VisibleAnywhere, Category = "PF Volume - Debug")
	TArray<FGridCell> Cells = {};

	UPROPERTY(VisibleAnywhere, Category = "PF Volume - Debug")
	FIntVector CellCountsPerAxis = FIntVector(0);

	UPROPERTY(Transient)
	bool bIsUpdatingPoints = false;

public:
	APFVolume();

	// Returns nearest grid cell to the passed in point
	UFUNCTION(BlueprintPure, Category = "PF Volume")
	const FGridCell& GetNearestGridCell(const FVector& Point);

	// Rounds passed in point to grid size, and returns cell index of that point
	UFUNCTION(BlueprintPure, Category = "PF Volume")
	const int32 GetNearestCellIndex(const FVector& Point) const;

private:
	UFUNCTION(CallInEditor, Category = "PF Volume - Debug")
	void PopulateGrid();

#if WITH_EDITOR
	void PostEditMove(bool bFinished) override;
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	void EditorApplyRotation(const FRotator& DeltaRotation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;
#endif

	friend class FPFVolumeDebugVisualizer;

};
