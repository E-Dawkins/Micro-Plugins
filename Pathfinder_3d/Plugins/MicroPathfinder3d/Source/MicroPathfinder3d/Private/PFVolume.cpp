// Fill out your copyright notice in the Description page of Project Settings.


#include "PFVolume.h"

#include "PFVolumeDebugComponent.h"

APFVolume::APFVolume()
{
    DebugComp = CreateDefaultSubobject<UPFVolumeDebugComponent>(TEXT("PF Volume Debug Component"));
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

const FIntVector APFVolume::GetNearestCellIndicesOfType(const FVector& Point, ENodeType NodeType) const
{
	FIntVector Out = GetNearestCellIndices(Point);

	if (NodeType != ENodeType::None)
	{
		// Find first node of 'NodeType'
		int32 StartingIndex = Nodes.GetIndex(Out);

		TQueue<int32> IndexesToSearch;
		IndexesToSearch.Enqueue(StartingIndex);

		TBitArray SearchedIndexes;
		SearchedIndexes.Init(false, Nodes.GetNodeCount());
		SearchedIndexes[StartingIndex] = true;

		TArray<int32> Neighbours;
		Neighbours.Reserve(6); // We only check orthognal neighbours

		int32 CurrentIndex;
		while (IndexesToSearch.Dequeue(CurrentIndex))
		{
			if (Nodes[CurrentIndex] == NodeType)
				return Nodes.GetAxisIndices(CurrentIndex);
			
			GetNeighbours(CurrentIndex, false, Neighbours);

			for (int32 N : Neighbours)
			{
				if (!SearchedIndexes[N])
				{
					IndexesToSearch.Enqueue(N);
					SearchedIndexes[N] = true;
				}
			}
		}
	}

	return Out;
}

const FVector APFVolume::GetWorldPositionFromAxisIndices(const FIntVector& AxisIndices) const
{
	const FVector BoxExtent = GetBounds().BoxExtent;
	const FVector StartPosition = GetActorLocation() - BoxExtent;

	return StartPosition + (FVector(AxisIndices) * CellSize);
}

#include <queue>

TArray<FVector> APFVolume::FindPathTo(const FVector& Start, const FVector& Goal, ENodeType NodeType, bool bIncludeDiagonals) const
{
	double StartTime = FPlatformTime::Seconds();

	/*const FIntVector StartIndices = GetNearestCellIndices(Start);
	const FIntVector GoalIndices = GetNearestCellIndices(Goal);*/
	const FIntVector StartIndices = GetNearestCellIndicesOfType(Start, NodeType);
	const FIntVector GoalIndices = GetNearestCellIndicesOfType(Goal, NodeType);
	const int32 StartIndex = Nodes.GetIndex(StartIndices);
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
		FScore[StartIndex] = Heuristic(StartIndices, GoalIndices);
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
	Neighbours.Reserve(bIncludeDiagonals ? 26 : 6);

	// On the off chance that the proceeding while loop instantly exits
	// we store this outside it to reconstruct the path afterwards
	int32 Current = StartIndex;

	while (!OpenSet.empty())
	{
		Current = OpenSet.top();
		OpenSet.pop();

		if (Current == GoalIndex)
			break;

		// Only access current node data once per neighbour loop
		const float& GScoreCurrent = GScore[Current];
		const FIntVector CurrentIndices = Nodes.GetAxisIndices(Current);

		GetNeighbours(Current, bIncludeDiagonals, Neighbours);
		for (int32 N : Neighbours)
		{
			// Check if 1. we are looking for a specific node type, and 2. this node is of that type
			if (NodeType != ENodeType::None && Nodes[N] != NodeType)
				continue;

			// Lazy init neighbour
			if (!Visited[N])
			{
				GScore[N] = FLT_MAX;
				FScore[N] = FLT_MAX;
				CameFrom[N] = INDEX_NONE;
				Visited[N] = true;
			}

			const FIntVector NeighbourIndices = Nodes.GetAxisIndices(N);
			const float StepCost = MovementCost(CurrentIndices, NeighbourIndices);

			const float TentativeGScore = GScoreCurrent + StepCost;
			if (TentativeGScore < GScore[N])
			{
				CameFrom[N] = Current;
				GScore[N] = TentativeGScore;
				FScore[N] = TentativeGScore + Heuristic(NeighbourIndices, GoalIndices);

				// Allow duplicates as eventually we *should* reach the goal
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

float APFVolume::Heuristic(const FIntVector& CurrentNodeIndices, const FIntVector& GoalIndices) const
{
	const float Dx = FMath::Abs(CurrentNodeIndices.X - GoalIndices.X);
	const float Dy = FMath::Abs(CurrentNodeIndices.Y - GoalIndices.Y);
	const float Dz = FMath::Abs(CurrentNodeIndices.Z - GoalIndices.Z);

	switch (CostHeuristic)
	{
		case ECostHeuristic::TrueEuclidean:
		{
			return FMath::Sqrt(Dx*Dx + Dy*Dy + Dz*Dz);
		}
		case ECostHeuristic::EuclideanSquared:
		{
			return Dx*Dx + Dy*Dy + Dz*Dz;
		}
		case ECostHeuristic::Diagonal3d:
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

void APFVolume::GetNeighbours(int32 NodeIndex, bool bIncludeDiagonals, TArray<int32>& Out) const
{
	Out.Reset();

	const FIntVector NodeIndices = Nodes.GetAxisIndices(NodeIndex);
	FIntVector NeighbourIndices = {};

	static constexpr int32 Offsets[26][3] = {
		// Orthogonal neighbours
		{ -1,  0,  0 }, {  0, -1,  0 }, {  0,  0, -1 },
		{  1,  0,  0 }, {  0,  1,  0 }, {  0,  0,  1 },

		// Diagonal neighbours
		{ -1, -1, -1 }, { -1,  0, -1 }, { -1,  1, -1 },
		{ -1, -1,  0 },					{ -1,  1,  0 },
		{ -1, -1,  1 },	{ -1,  0,  1 },	{ -1,  1,  1 },

		{  0, -1, -1 },					{  0,  1, -1 },
		{  0, -1,  1 },					{  0,  1,  1 },

		{  1, -1, -1 }, {  1,  0, -1 }, {  1,  1, -1 },
		{  1, -1,  0 },					{  1,  1,  0 },
		{  1, -1,  1 },	{  1,  0,  1 },	{  1,  1,  1 }
	};

	for (int32 OffsetIndex = 0; OffsetIndex < (bIncludeDiagonals ? 26 : 6); OffsetIndex++)
	{
		const int32* Offset = Offsets[OffsetIndex];

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

float APFVolume::MovementCost(const FIntVector& A, const FIntVector& B) const
{
	const int dx = FMath::Abs(A.X - B.X);
	const int dy = FMath::Abs(A.Y - B.Y);
	const int dz = FMath::Abs(A.Z - B.Z);

	const int sum = dx + dy + dz;

	if (sum == 1) return 1.f;            // straight
	if (sum == 2) return 1.41421356f;    // 2-axis diagonal
	return 1.73205081f;                  // 3-axis diagonal
}

void APFVolume::CheckGridForCollisions()
{
	// First pass set all node types
	for (const auto& [AxisIndices, Value] : Nodes)
	{
		const FVector WorldPosition = GetWorldPositionFromAxisIndices(AxisIndices);

		bool bIsOverlapping = GetWorld()->OverlapBlockingTestByChannel(WorldPosition, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeBox(CellSize * 0.5f));

		Nodes[AxisIndices] = (bIsOverlapping ? ENodeType::InCollision : ENodeType::Open);
	}

	// We then set nodes that are near collision
	TArray<int32> Neighbours;
	Neighbours.Reserve(26);

	for (int32 NodeIndex = 0; NodeIndex < Nodes.GetNodeCount(); NodeIndex++)
	{
		if (Nodes[NodeIndex] == ENodeType::InCollision)
		{
			// Set each open neighbour to be near collision,
			// unfortunately we do need diagonals incase the collision is an outside corner
			// 
			// i.e. X is the missed node if it is an outside corner and we skip diagonals
			// * * * X
			// - - - *
			//     | *
			//     | *

			GetNeighbours(NodeIndex, true, Neighbours);

			for (int32 N : Neighbours)
			{
				if (Nodes[N] == ENodeType::Open)
				{
					Nodes[N] = ENodeType::NearCollision;
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

		// Only update nodes if cell count has changed,
		// or node count has desynced from real array size
		if (CellCountsPerAxis != MaxCellCount || Nodes.GetNodeCount() != Nodes.GetRawNodeCount())
		{
			CellCountsPerAxis = MaxCellCount;
			Nodes.Resize(CellCountsPerAxis, ENodeType::Open);
		}

		CheckGridForCollisions();
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
