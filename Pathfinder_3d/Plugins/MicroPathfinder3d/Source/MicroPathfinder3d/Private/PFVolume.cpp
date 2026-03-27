// Fill out your copyright notice in the Description page of Project Settings.


#include "PFVolume.h"

#include "PFVolumeDebugComponent.h"

APFVolume::APFVolume()
{
#if WITH_EDITORONLY_DATA
    DebugComp = CreateDefaultSubobject<UPFVolumeDebugComponent>(TEXT("PF Volume Debug Component"));
#endif
}

const FIntVector APFVolume::GetNearestCellIndices(const FVector& Point) const
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

	return PointRounded;
}

const FVector APFVolume::GetWorldPositionFromAxisIndices(const FIntVector& AxisIndices) const
{
	const FVector BoxExtent = GetBounds().BoxExtent;
	const FVector StartPosition = GetActorLocation() - BoxExtent;

	return StartPosition + (FVector(AxisIndices) * CellSize);
}

#include <queue>

TArray<FVector> APFVolume::FindPathTo(const FVector& Start, const FVector& Goal)
{
	const FIntVector StartIndices = GetNearestCellIndices(Start);
	const FIntVector GoalIndices = GetNearestCellIndices(Goal);

	struct FFloatDefaultedToInfinity { float Value = INFINITY; };
	using ScoreMap = TMap<FIntVector, FFloatDefaultedToInfinity>;

	ScoreMap GScore = {};
	GScore.Add(StartIndices, { 0.f });

	ScoreMap FScore = {};
	FScore.Add(StartIndices, { Heuristic(StartIndices, GoalIndices) });

	struct FNodeCompare
	{
		const ScoreMap& FScore;

		FNodeCompare(const ScoreMap& InFScore)
			: FScore(InFScore) {}

		bool operator () (const FIntVector& A, const FIntVector& B) const
		{
			auto ItA = FScore.Find(A);
			auto ItB = FScore.Find(B);

			float FA = (ItA ? ItA->Value : INFINITY);
			float FB = (ItB ? ItB->Value : INFINITY);

			return FScore[A].Value > FScore[B].Value;
		}
	};
	using TMinHeap = std::priority_queue<FIntVector, std::vector<FIntVector>, FNodeCompare>;
	TMinHeap OpenSet{ FNodeCompare(FScore) };
	OpenSet.push(StartIndices);

	// Required for checking if node is in open set or not
	TSet<FIntVector> InOpenSet = { StartIndices };

	TMap<FIntVector, FIntVector> CameFrom = {};

	// On the off chance that the proceeding while loop instantly exits
	// we store this outside it to reconstruct the path afterwards
	FIntVector Current = StartIndices;

	while (!OpenSet.empty())
	{
		Current = OpenSet.top();
		if (Current == GoalIndices)
			break;

		OpenSet.pop();
		InOpenSet.Remove(Current);

		TArray<FIntVector> Neighbours = {};
		GetNeighbours(Current, Neighbours);

		for (const FIntVector& N : Neighbours)
		{
			// Failsafe, not sure what happens if you do +1.f to infinity...
			auto* GCurrentPtr = GScore.Find(Current);
			if (!GCurrentPtr || GCurrentPtr->Value == INFINITY) continue;

			// Currently each edge is uniform 1.f in weight
			float TentativeGScore = GCurrentPtr->Value + 1.f;

			auto* GNPtr = GScore.Find(N);
			float GN = (GNPtr ? GNPtr->Value : INFINITY);

			if (TentativeGScore < GN)
			{
				CameFrom.Add(N, Current);
				GScore.Add(N, { TentativeGScore });
				FScore.Add(N, { TentativeGScore + Heuristic(N, GoalIndices) });

				if (!InOpenSet.Contains(N))
				{
					OpenSet.push(N);
					InOpenSet.Add(N);
				}
			}
		}
	}

	// Reconstruct path
	TArray<FVector> Path = { GetWorldPositionFromAxisIndices(Current) };
	while (CameFrom.Contains(Current))
	{
		Current = CameFrom[Current];
		Path.Insert(GetWorldPositionFromAxisIndices(Current), 0);
	}

	return Path;
}

float APFVolume::Heuristic(const FIntVector& StartIndices, const FIntVector& GoalIndices) const
{
	const FVector& Start = GetWorldPositionFromAxisIndices(StartIndices);
	const FVector& Goal = GetWorldPositionFromAxisIndices(GoalIndices);

	switch (CostHeuristic)
	{
		case ECostHeuristic::EuclideanSquared:
		{
			return FVector::DistSquared(Start, Goal);
		}
		case ECostHeuristic::_3dDiagonal:
		{
			const float Dx = FMath::Abs(Start.X - Goal.X);
			const float Dy = FMath::Abs(Start.Y - Goal.Y);
			const float Dz = FMath::Abs(Start.Z - Goal.Z);

			const float D1 = FMath::Min3(Dx, Dy, Dz);
			const float D3 = FMath::Max3(Dx, Dy, Dz);
			const float D2 = Dx + Dy + Dz - D1 - D3;

			return D1 * 1.73205081f			// ~ root 3
				+ (D2 - D1) * 1.41421356f	// ~ root 2
				+ (D3 - D2);				// straight steps
		}
		case ECostHeuristic::Chebyshev:
		{
			const float Dx = FMath::Abs(Start.X - Goal.X);
			const float Dy = FMath::Abs(Start.Y - Goal.Y);
			const float Dz = FMath::Abs(Start.Z - Goal.Z);

			return FMath::Max3(Dx, Dy, Dz);
		}
		default:
		{
			// Default to Manhattan distance
			return FMath::Abs(Start.X - Goal.X)
				+ FMath::Abs(Start.Y - Goal.Y)
				+ FMath::Abs(Start.Z - Goal.Z);
		}
	}
}

void APFVolume::GetNeighbours(const FIntVector& AxisIndices, TArray<FIntVector>& Out)
{
	Out.Empty();

	for (int32 XOffset = -1; XOffset <= 1; XOffset++)
	{
		for (int32 YOffset = -1; YOffset <= 1; YOffset++)
		{
			for (int32 ZOffset = -1; ZOffset <= 1; ZOffset++)
			{
				// Skip ourself :)
				if (XOffset == 0 && YOffset == 0 && ZOffset == 0) continue;

				const FIntVector NeighbourIndices = AxisIndices + FIntVector{ XOffset, YOffset, ZOffset };
				if (0 <= NeighbourIndices.X && NeighbourIndices.X < CellCountsPerAxis.X &&
					0 <= NeighbourIndices.Y && NeighbourIndices.Y < CellCountsPerAxis.Y &&
					0 <= NeighbourIndices.Z && NeighbourIndices.Z < CellCountsPerAxis.Z)
				{
					Out.Add(NeighbourIndices);
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

		const FVector BoxExtent = GetBounds().BoxExtent;
		const FIntVector MaxCellCount = FIntVector((BoxExtent * 2.f) / CellSize) + FIntVector(1);

		// Only update nodes if cell count has changed
		if (CellCountsPerAxis != MaxCellCount)
		{
			CellCountsPerAxis = MaxCellCount;
			Nodes.Resize(CellCountsPerAxis, ENodeType::OpenAir);
		}
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
