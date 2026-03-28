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
	double StartTime = FPlatformTime::Seconds();

	const int32 StartIndex = Nodes.GetIndex(GetNearestCellIndices(Start));
	const FIntVector GoalIndices = GetNearestCellIndices(Goal);
	const int32 GoalIndex = Nodes.GetIndex(GoalIndices);
	const int32 NodeCount = Nodes.GetNodeCount();

	TArray<float> GScore;
	TArray<float> FScore;
	TArray<int32> CameFrom;
	TArray<uint8> Visited;

	// Allocate arrays now, but do *not* initialize them
	{
		GScore.SetNumUninitialized(NodeCount);
		FScore.SetNumUninitialized(NodeCount);
		CameFrom.SetNumUninitialized(NodeCount);
		Visited.SetNumZeroed(NodeCount);
	}

	// Lazy init start node
	{
		GScore[StartIndex] = 0.f;
		FScore[StartIndex] = Heuristic(StartIndex, GoalIndices);
		CameFrom[StartIndex] = INDEX_NONE;
		Visited[StartIndex] = true;
	}

	struct FNodeCompare
	{
		const TArray<float>& FScore;
		bool operator()(int32 A, int32 B) const { return FScore[A] > FScore[B]; }
	};

	std::priority_queue<int32, std::vector<int32>, FNodeCompare> OpenSet{ FNodeCompare(FScore) };
	OpenSet.push(StartIndex);

	TArray<int32> Neighbours = {};
	Neighbours.Reserve(26);

	// On the off chance that the proceeding while loop instantly exits
	// we store this outside it to reconstruct the path afterwards
	int32 Current = StartIndex;

	while (!OpenSet.empty())
	{
		Current = OpenSet.top();
		OpenSet.pop();

		if (Current == GoalIndex)
			break;

		// Only access current gscore once per neighbour loop
		const float& GScoreCurrent = GScore[Current];

		// If edge weights are ever added, move this back into the neighbour loop
		// Currently each edge is uniform 1.f in weight
		const float TentativeGScore = GScoreCurrent + 1.f;

		GetNeighbours(Current, Neighbours);
		for (int32 N : Neighbours)
		{
			// Lazy init neighbour
			if (!Visited[N])
			{
				GScore[N] = FLT_MAX;
				FScore[N] = FLT_MAX;
				CameFrom[N] = INDEX_NONE;
				Visited[N] = true;
			}

			if (TentativeGScore < GScore[N])
			{
				CameFrom[N] = Current;
				GScore[N] = TentativeGScore;
				FScore[N] = TentativeGScore + Heuristic(N, GoalIndices);

				// Allow duplicates as eventually we *will* reach the goal
				OpenSet.push(N);
			}
		}
	}

	// Reconstruct path
	TArray<FVector> Path = {};
	while (Current != INDEX_NONE)
	{
		Path.Add(GetWorldPositionFromAxisIndices(Nodes.GetAxisIndices(Current)));
		Current = CameFrom[Current];
	}

	Algo::Reverse(Path);

	double EndTime = FPlatformTime::Seconds();
	double ElapsedMs = (EndTime - StartTime) * 1000.0;
	UE_LOG(LogTemp, Warning, TEXT("%hs took %.6f ms"), __FUNCTION__, ElapsedMs);

	return Path;
}

float APFVolume::Heuristic(int32 CurrentNodeIndex, const FIntVector& GoalIndices) const
{
	const FIntVector CurrentNodeIndices = Nodes.GetAxisIndices(CurrentNodeIndex);

	const float Dx = FMath::Abs(CurrentNodeIndices.X - GoalIndices.X);
	const float Dy = FMath::Abs(CurrentNodeIndices.Y - GoalIndices.Y);
	const float Dz = FMath::Abs(CurrentNodeIndices.Z - GoalIndices.Z);

	switch (CostHeuristic)
	{
		case ECostHeuristic::EuclideanSquared:
		{
			return Dx*Dx + Dy*Dy + Dz*Dz;
		}
		case ECostHeuristic::_3dDiagonal:
		{
			const float Min = FMath::Min3(Dx, Dy, Dz);
			const float Max = FMath::Max3(Dx, Dy, Dz);
			const float Mid = Dx + Dy + Dz - Min - Max;

			return Min * 1.73205081f			// ~ root 3
				+ (Mid - Min) * 1.41421356f	// ~ root 2
				+ (Max - Mid);				// straight steps
		}
		case ECostHeuristic::Chebyshev:
		{
			return FMath::Max3(Dx, Dy, Dz);
		}
		default:
		{
			// Default to Manhattan distance
			return Dx + Dy + Dz;
		}
	}
}

void APFVolume::GetNeighbours(int32 NodeIndex, TArray<int32>& Out)
{
	Out.Reset();

	const FIntVector NodeIndices = Nodes.GetAxisIndices(NodeIndex);
	FIntVector NeighbourIndices = {};

	static constexpr int32 Offsets[26][3] = {
		{ -1, -1, -1 }, { -1,  0, -1 }, { -1,  1, -1 },
		{ -1, -1,  0 },	{ -1,  0,  0 },	{ -1,  1,  0 },
		{ -1, -1,  1 },	{ -1,  0,  1 },	{ -1,  1,  1 },

		{  0, -1, -1 }, {  0,  0, -1 }, {  0,  1, -1 },
		{  0, -1,  0 },	                {  0,  1,  0 },
		{  0, -1,  1 },	{  0,  0,  1 },	{  0,  1,  1 },

		{  1, -1, -1 }, {  1,  0, -1 }, {  1,  1, -1 },
		{  1, -1,  0 },	{  1,  0,  0 },	{  1,  1,  0 },
		{  1, -1,  1 },	{  1,  0,  1 },	{  1,  1,  1 }
	};

	for (const int32* Offset : Offsets)
	{
		NeighbourIndices = {
			NodeIndices.X + Offset[0],
			NodeIndices.Y + Offset[1],
			NodeIndices.Z + Offset[2],
		};

		if (0 <= NeighbourIndices.X && NeighbourIndices.X < CellCountsPerAxis.X &&
			0 <= NeighbourIndices.Y && NeighbourIndices.Y < CellCountsPerAxis.Y &&
			0 <= NeighbourIndices.Z && NeighbourIndices.Z < CellCountsPerAxis.Z)
		{
			Out.Emplace(Nodes.GetIndex(NeighbourIndices));
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
