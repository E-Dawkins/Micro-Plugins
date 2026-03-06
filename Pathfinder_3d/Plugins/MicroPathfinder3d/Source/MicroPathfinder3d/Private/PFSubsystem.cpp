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

	UBrushComponent* Brush = Cast<UBrushComponent>(Volume->GetBrushComponent());
	if (!Brush)
		return FLT_MAX;

	UBodySetup* BodySetup = Brush->GetBodySetup();
	if (!BodySetup)
		return FLT_MAX;

	const FTransform& BrushTransform = Brush->GetComponentTransform();

	float ClosestDistance = FLT_MAX;
	bool bInside = false;

	for (const FKConvexElem& Convex : BodySetup->AggGeom.ConvexElems)
	{
		FVector ClosestPointWS = {}, ClosestNormalWS = {};
		const float DistanceToHull = Convex.GetClosestPointAndNormal(Point, BrushTransform, ClosestPointWS, ClosestNormalWS);

		if (DistanceToHull < ClosestDistance)
			ClosestDistance = DistanceToHull;

		if (DistanceToHull == 0.f)
			bInside = true;
	}

	return (bInside ? -ClosestDistance : ClosestDistance);
}
