// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "PFNode.h"
#include "PFVolume.generated.h"

UENUM(BlueprintType)
enum class ECostHeuristic : uint8
{
	Manhattan			UMETA(ToolTip = "Slow + worst path quality"),
	TrueEuclidean		UMETA(ToolTip = "Fast + average path quality"),
	EuclideanSquared	UMETA(ToolTip = "Fastest + decent path quality"),
	Diagonal3d			UMETA(ToolTip = "Slowest + best path quality", DisplayName = "3d Diagonal"),
	Chebyshev			UMETA(ToolTip = "Average + poor path quality")
};

UCLASS(meta = (DisplayName = "PF Volume"))
class MICROPATHFINDER3D_API APFVolume : public AVolume
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "PF Volume")
	FVector CellSize = FVector(100.f);

	UPROPERTY(EditAnywhere, Category = "PF Volume")
	ECostHeuristic CostHeuristic = ECostHeuristic::Diagonal3d;

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

	UFUNCTION(BlueprintCallable, Category = "PF Volume", meta = (AdvancedDisplay = "NodeType"))
	TArray<FVector> FindPathTo(const FVector& Start, const FVector& Goal, ENodeType NodeType = ENodeType::None);

private:
	FORCEINLINE float Heuristic(const FIntVector& CurrentNodeIndices, const FIntVector& GoalIndices) const;
	void GetNeighbours(int32 NodeIndex, TArray<int32>& Out);
	float MovementCost(const FIntVector& A, const FIntVector& B);

	void CheckGridForCollisions();

#if WITH_EDITOR
	void PostEditMove(bool bFinished) override;
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	void EditorApplyRotation(const FRotator& DeltaRotation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;
#endif

	friend class FPFVolumeDebugVisualizer;

};
