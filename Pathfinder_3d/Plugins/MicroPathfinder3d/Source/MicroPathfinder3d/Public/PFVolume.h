// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "PFVolume.generated.h"

USTRUCT()
struct FGridCell
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
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

	UPROPERTY(Transient)
	bool bIsUpdatingPoints = false;

public:
	APFVolume();

private:
	UFUNCTION(CallInEditor, Category = "PF Volume - Debug")
	void PopulateGrid();

#if WITH_EDITOR
	void PostEditMove(bool bFinished);
#endif
	
	friend class FPFVolumeDebugVisualizer;

};
