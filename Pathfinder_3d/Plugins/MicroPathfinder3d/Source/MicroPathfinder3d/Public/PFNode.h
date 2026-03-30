// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PFNode.generated.h"

UENUM(BlueprintType)
enum class ENodeType : uint8
{
	None,
	Open,
	InCollision,
	NearCollision
};

USTRUCT(BlueprintType)
struct FNodeArray
{
	GENERATED_BODY()

private:
	// We do not display this in editor, can be very laggy
	UPROPERTY(meta = (HideInDetailPanel))
	TArray<ENodeType> Array = {};

	UPROPERTY(EditAnywhere)
	FIntVector AxisSizes = {};

	UPROPERTY(EditAnywhere)
	int32 NodeCount = 0;

public:
	void Resize(const FIntVector& InAxisSizes, ENodeType DefaultValue)
	{
		AxisSizes = InAxisSizes;

		Array.Empty();
		Array.Init(DefaultValue, AxisSizes.X * AxisSizes.Y * AxisSizes.Z);

		NodeCount = Array.Num();
	}

	// Gets the pre-calculated node count
	int32 GetNodeCount() const { return NodeCount; };

	// Gets the underlying array's count
	int32 GetRawNodeCount() const { return Array.Num(); }

	FORCEINLINE int32 GetIndex(const FIntVector& Indices) const
	{
		return Indices.X + AxisSizes.X * (Indices.Y + AxisSizes.Y * Indices.Z);
	}

	FORCEINLINE const FIntVector GetAxisIndices(int32 Index) const
	{
		const int32 XY = AxisSizes.X * AxisSizes.Y;

		FIntVector Out = {};

		Out.Z = Index / XY;
		const int32 Rem = Index - Out.Z * XY;

		Out.Y = Rem / AxisSizes.X;
		Out.X = Rem - Out.Y * AxisSizes.X;

		return Out;
	}

	ENodeType& operator [] (const FIntVector& Indices)
	{
		return Array[GetIndex(Indices)];
	}

	const ENodeType& operator [] (const FIntVector& Indices) const
	{
		return Array[GetIndex(Indices)];
	}

	ENodeType& operator [] (int32 Index)
	{
		return Array[Index];
	}

	const ENodeType& operator [] (int32 Index) const
	{
		return Array[Index];
	}

	struct FIterator
	{
		FNodeArray* NodeArray;
		int32 Index;
		int32 ElemCount;

		bool operator != (const FIterator& Other) const { return Index != Other.Index; }
		FIterator& operator ++ ()
		{
			++Index;
			return *this;
		}

		auto operator * () const
		{
			check(Index < ElemCount);

			int32 X = Index % NodeArray->AxisSizes.X;
			int32 Y = (Index / NodeArray->AxisSizes.X) % NodeArray->AxisSizes.Y;
			int32 Z = Index / (NodeArray->AxisSizes.X * NodeArray->AxisSizes.Y);

			return TTuple<FIntVector, ENodeType&>(
				FIntVector{ X, Y, Z }, NodeArray->Array[Index]
			);
		}
	};

	struct FConstIterator
	{
		const FNodeArray* NodeArray;
		int32 Index;
		int32 ElemCount;

		bool operator != (const FConstIterator& Other) const { return Index != Other.Index; }
		FConstIterator& operator ++ ()
		{
			++Index;
			return *this;
		}

		auto operator * () const
		{
			check(Index < ElemCount);

			int32 X = Index % NodeArray->AxisSizes.X;
			int32 Y = (Index / NodeArray->AxisSizes.X) % NodeArray->AxisSizes.Y;
			int32 Z = Index / (NodeArray->AxisSizes.X * NodeArray->AxisSizes.Y);

			return TTuple<FIntVector, const ENodeType&>(
				FIntVector{ X, Y, Z }, NodeArray->Array[Index]
			);
		}
	};

	FIterator begin() { return FIterator{ this, 0, Array.Num() }; }
	FIterator end() { return FIterator{ this, Array.Num(), Array.Num() }; }

	FConstIterator begin() const { return FConstIterator{ this, 0, Array.Num() }; }
	FConstIterator end() const { return FConstIterator{ this, Array.Num(), Array.Num() }; }

	FConstIterator cbegin() const { return FConstIterator{ this, 0, Array.Num() }; }
	FConstIterator cend() const { return FConstIterator{ this, Array.Num(), Array.Num() }; }
};
