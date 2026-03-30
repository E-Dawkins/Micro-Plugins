#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PFNode.h"
#include "PFVolumeDebugComponent.generated.h"

USTRUCT(BlueprintType)
struct FNodeStyle
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    ENodeType Type;

    UPROPERTY(EditAnywhere)
    FLinearColor Color;
};

USTRUCT(BlueprintType)
struct FNodeStyleArray
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, EditFixedSize, meta=(EditFixedOrder))
    TArray<FNodeStyle> Items = {
        { ENodeType::Open, FColor::Cyan },
        { ENodeType::InCollision, FColor::Red },
        { ENodeType::NearCollision, FColor::Orange }
    };
};

UCLASS()
class MICROPATHFINDER3D_API UPFVolumeDebugComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    bool bVisualizePoints = false;

    UPROPERTY(EditAnywhere)
    FNodeStyleArray NodeStyles;
};