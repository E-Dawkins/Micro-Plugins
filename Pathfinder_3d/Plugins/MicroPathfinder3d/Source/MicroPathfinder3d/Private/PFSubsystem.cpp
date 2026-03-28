// Fill out your copyright notice in the Description page of Project Settings.


#include "PFSubsystem.h"

#include <Components/BrushComponent.h>
#include <Kismet/GameplayStatics.h>
#include <PhysicsEngine/BodySetup.h>

#include "PFVolume.h"

void UPFSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	// This function is called *before* any actor BeginPlay is run
	TArray<AActor*> FoundVolumes = {};
	UGameplayStatics::GetAllActorsOfClass(&InWorld, APFVolume::StaticClass(), FoundVolumes);

	for (AActor* Volume : FoundVolumes)
	{
		if (APFVolume* PFVolume = Cast<APFVolume>(Volume))
		{
			PFVolumes.Add(PFVolume);
		}
	}
}

APFVolume* UPFSubsystem::GetNearestPFVolume(const FVector& Point)
{
	APFVolume* NearestVolume = nullptr;
	float NearestAbsDistance = FLT_MAX;

	for (APFVolume* PFVolume : PFVolumes)
	{
		float SignedDistance = GetDistanceToVolumeSurface(PFVolume, Point);

		float AbsDistance = FMath::Abs(SignedDistance);
		if (AbsDistance < NearestAbsDistance)
		{
			NearestAbsDistance = AbsDistance;
			NearestVolume = PFVolume;
		}
	}

	return NearestVolume;
}

float UPFSubsystem::GetDistanceToVolumeSurface(APFVolume* Volume, const FVector& Point)
{
	if (!IsValid(Volume))
		return FLT_MAX;

	FVector Origin, Extents;
	Volume->GetActorBounds(false, Origin, Extents);

	const FVector Local = Point - Origin;

	const FVector Delta = Local.GetAbs() - Extents;

	const float OutsideDist = FVector(
		FMath::Max(Delta.X, 0.f),
		FMath::Max(Delta.Y, 0.f),
		FMath::Max(Delta.Z, 0.f)
	).Length();

	const float InsideDist = FMath::Min(Delta.GetMax(), 0.f);

	return (OutsideDist > 0.f ? OutsideDist : InsideDist);
}
