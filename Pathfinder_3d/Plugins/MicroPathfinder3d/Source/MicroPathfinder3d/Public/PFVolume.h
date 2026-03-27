// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "PFNode.h"
#include "PFVolume.generated.h"

UENUM(BlueprintType)
enum class ECostHeuristic : uint8
{
	Manhattan UMETA(ToolTip = "Fast + poor path quality"),
	EuclideanSquared UMETA(ToolTip = "Fast + good path quality"),
	_3dDiagonal UMETA(ToolTip = "Fast + best path quality"),
	Chebyshev UMETA(ToolTip = "Very fast + average path quality")
};

UCLASS(meta = (DisplayName = "PF Volume"))
class MICROPATHFINDER3D_API APFVolume : public AVolume
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "PF Volume")
	FVector CellSize = FVector(100.f);

	UPROPERTY(EditAnywhere, Category = "PF Volume")
	ECostHeuristic CostHeuristic = ECostHeuristic::_3dDiagonal;

private:
	UPROPERTY(VisibleAnywhere, Category = "PF Volume - Debug")
	FIntVector CellCountsPerAxis = FIntVector(0);

	UPROPERTY(VisibleAnywhere, Category = "PF Volume - Debug")
	FNodeArray Nodes = {};

	UPROPERTY(Transient)
	bool bIsUpdatingPoints = false;

	UPROPERTY(VisibleAnywhere, Instanced)
	class UPFVolumeDebugComponent* DebugComp = nullptr;

public:
	APFVolume();

	// Returns passed in point rounded and clamped to grid size
	UFUNCTION(BlueprintPure, Category = "PF Volume")
	const FIntVector GetNearestCellIndices(const FVector& Point) const;

	// Returns world position from axis indices
	UFUNCTION(BlueprintPure, Category = "PF Volume")
	const FVector GetWorldPositionFromAxisIndices(const FIntVector& AxisIndices) const;

	UFUNCTION(BlueprintCallable, Category = "PF Volume")
	TArray<FVector> FindPathTo(const FVector& Start, const FVector& Goal);

private:
	float Heuristic(const FIntVector& StartIndices, const FIntVector& GoalIndices) const;
	void GetNeighbours(const FIntVector& AxisIndices, TArray<FIntVector>& Out);

#if WITH_EDITOR
	void PostEditMove(bool bFinished) override;
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	void EditorApplyRotation(const FRotator& DeltaRotation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;
#endif

	friend class FPFVolumeDebugVisualizer;

};
