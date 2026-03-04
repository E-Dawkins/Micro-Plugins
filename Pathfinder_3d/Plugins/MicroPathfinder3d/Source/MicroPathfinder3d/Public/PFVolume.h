// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "PFVolume.generated.h"

USTRUCT()
struct FGridCell
{
	GENERATED_BODY()

	FVector WorldPosition = {};
};

UCLASS()
class MICROPATHFINDER3D_API APFVolume : public AVolume
{
	GENERATED_BODY()

public:
	APFVolume();

	UPROPERTY(EditAnywhere, Category = "PF Volume")
	TArray<FGridCell> Cells = {};
	
};
