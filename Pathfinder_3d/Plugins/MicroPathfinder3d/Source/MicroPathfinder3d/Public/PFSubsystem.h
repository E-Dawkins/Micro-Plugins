// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PFSubsystem.generated.h"

class APFVolume;

UCLASS()
class MICROPATHFINDER3D_API UPFSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

private:
	TArray<APFVolume*> PFVolumes = {};

public:
	void OnWorldBeginPlay(UWorld& InWorld) override;

	UFUNCTION(BlueprintCallable, Category = "PF Subsystem")
	APFVolume* GetNearestPFVolume(const FVector& Point);
	
private:
	float GetDistanceToVolumeSurface(APFVolume* Volume, const FVector& Point);
};
