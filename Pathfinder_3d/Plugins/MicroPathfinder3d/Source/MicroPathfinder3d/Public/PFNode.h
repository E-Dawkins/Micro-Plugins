// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PFNode.generated.h"

UENUM(BlueprintType)
enum class ENodeType : uint8
{
	OpenAir,
	InsideWall,
	NearWall
};

USTRUCT(BlueprintType)
struct FNodeArray
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TArray<ENodeType> Array = {};

	UPROPERTY(EditAnywhere)
	FIntVector AxisSizes = {};

public:
	void Resize(const FIntVector& InAxisSizes, ENodeType DefaultValue)
	{
		AxisSizes = InAxisSizes;

		Array.Empty();
		Array.SetNum(AxisSizes.X * AxisSizes.Y * AxisSizes.Z);

		for (auto& Elem : Array)
		{
			Elem = DefaultValue;
		}
	}

	int32 GetIndex(const FIntVector& Indices) const
	{
		return Indices.X + AxisSizes.X * (Indices.Y + AxisSizes.Y * Indices.Z);
	}

	ENodeType& operator () (const FIntVector& Indices)
	{
		return Array[GetIndex(Indices)];
	}

	const ENodeType& operator () (const FIntVector& Indices) const
	{
		return Array[GetIndex(Indices)];
	}

	struct FIterator
	{
		FNodeArray* NodeArray;
		int32 Index;

		bool operator != (const FIterator& Other) const { return Index != Other.Index; }
		void operator ++ () { ++Index; }

		auto operator * () const
		{
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

		bool operator != (const FConstIterator& Other) const { return Index != Other.Index; }
		void operator ++ () { ++Index; }

		auto operator * () const
		{
			int32 X = Index % NodeArray->AxisSizes.X;
			int32 Y = (Index / NodeArray->AxisSizes.X) % NodeArray->AxisSizes.Y;
			int32 Z = Index / (NodeArray->AxisSizes.X * NodeArray->AxisSizes.Y);

			return TTuple<FIntVector, const ENodeType&>(
				FIntVector{ X, Y, Z }, NodeArray->Array[Index]
			);
		}
	};

	FIterator begin() { return FIterator{ this, 0 }; }
	FIterator end() { return FIterator{ this, AxisSizes.X * AxisSizes.Y * AxisSizes.Z }; }

	FConstIterator begin() const { return FConstIterator{ this, 0 }; }
	FConstIterator end() const { return FConstIterator{ this, AxisSizes.X * AxisSizes.Y * AxisSizes.Z }; }

	FConstIterator cbegin() const { return FConstIterator{ this, 0 }; }
	FConstIterator cend() const { return FConstIterator{ this, AxisSizes.X * AxisSizes.Y * AxisSizes.Z }; }
};
