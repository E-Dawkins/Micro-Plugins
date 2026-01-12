#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "MicroUnitConverterBPLibrary.generated.h"

USTRUCT(BlueprintType)
struct FPrintOptions
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	int32 MinDecimalPlaces = 0;

	UPROPERTY(BlueprintReadWrite)
	int32 MaxDecimalPlaces = 6;

	UPROPERTY(BlueprintReadWrite)
	bool bPrintToScreen = true;

	UPROPERTY(BlueprintReadWrite)
	bool bPrintToLog = true;
};

UENUM(BlueprintType)
enum class EDistanceUnit : uint8
{
	EDU_Micrometers		UMETA(DisplayName = "Micrometers"),
	EDU_Millimeters		UMETA(DisplayName = "Millimeters"),
	EDU_Centimeters		UMETA(DisplayName = "Centimeters"),
	EDU_Meters			UMETA(DisplayName = "Meters"),
	EDU_Kilometers		UMETA(DisplayName = "Kilometers"),
	EDU_Inches			UMETA(DisplayName = "Inches"),
	EDU_Feet			UMETA(DisplayName = "Feet"),
	EDU_Yards			UMETA(DisplayName = "Yards"),
	EDU_Miles			UMETA(DisplayName = "Miles"),
	EDU_Lightyears		UMETA(DisplayName = "Lightyears"),
};

UENUM(BlueprintType)
enum class EWeightUnit : uint8
{
	EWU_Micrograms		UMETA(DisplayName = "Micrograms"),
	EWU_Milligrams		UMETA(DisplayName = "Milligrams"),
	EWU_Grams			UMETA(DisplayName = "Grams"),
	EWU_Kilograms		UMETA(DisplayName = "Kilograms"),
	EWU_MetricTons		UMETA(DisplayName = "Metric Tons"),
	EWU_Ounces			UMETA(DisplayName = "Ounces"),
	EWU_Pounds			UMETA(DisplayName = "Pounds"),
	EWU_Stones			UMETA(DisplayName = "Stones"),
};

UCLASS()
class UMicroUnitConverterBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	// Converts value from one distance unit to another using the appropriate ratio
	UFUNCTION(BlueprintPure, Category = "Micro Unit Converter", meta = (DisplayName = "Convert Unit - Distance"))
	static double ConvertUnit_Distance(double InValue, EDistanceUnit InUnit, EDistanceUnit OutUnit);

	// Prints the converted distance value to screen / output log
	UFUNCTION(BlueprintCallable, Category = "Micro Unit Converter", meta = (DisplayName = "Print Conversion - Distance", DevelopmentOnly))
	static void PrintConversion_Distance(double InValue, EDistanceUnit InUnit, EDistanceUnit OutUnit, FPrintOptions PrintOptions);

	// Converts value from one weight unit to another using the appropriate ratio
	UFUNCTION(BlueprintPure, Category = "Micro Unit Converter", meta = (DisplayName = "Convert Unit - Weight"))
	static double ConvertUnit_Weight(double InValue, EWeightUnit InUnit, EWeightUnit OutUnit);

	// Prints the converted weight value to screen / output log
	UFUNCTION(BlueprintCallable, Category = "Micro Unit Converter", meta = (DisplayName = "Print Conversion - Weight", DevelopmentOnly))
	static void PrintConversion_Weight(double InValue, EWeightUnit InUnit, EWeightUnit OutUnit, FPrintOptions PrintOptions);

private:
	static void PrintImpl(double ConvertedValue, FString UnitSuffix, FPrintOptions PrintOptions);

private:
	// Output = Input * (RatioFrom/RatioTo)

	// These ratios are measured in centimeters, i.e. 1 mm = 0.1 cm
	inline static const TMap<EDistanceUnit, double> DistanceRatios =
	{
		{ EDistanceUnit::EDU_Micrometers,	0.0001 },
		{ EDistanceUnit::EDU_Millimeters,	0.1 },
		{ EDistanceUnit::EDU_Centimeters,	1.0 },
		{ EDistanceUnit::EDU_Meters,		100.0 },
		{ EDistanceUnit::EDU_Kilometers,	100000.0 },
		{ EDistanceUnit::EDU_Inches,		2.54 },
		{ EDistanceUnit::EDU_Feet,			30.48 },
		{ EDistanceUnit::EDU_Yards,			91.44 },
		{ EDistanceUnit::EDU_Miles,			160934.4 },
		{ EDistanceUnit::EDU_Lightyears,	946073047258004200.0 },	// this is the only 'rounded' ratio as lightyears are gigantic
	};

	inline static const TMap<EDistanceUnit, FString> DistanceSuffix = 
	{
		{ EDistanceUnit::EDU_Micrometers,	"µm" },
		{ EDistanceUnit::EDU_Millimeters,	"mm" },
		{ EDistanceUnit::EDU_Centimeters,	"cm" },
		{ EDistanceUnit::EDU_Meters,		"m" },
		{ EDistanceUnit::EDU_Kilometers,	"km" },
		{ EDistanceUnit::EDU_Inches,		"in" },
		{ EDistanceUnit::EDU_Feet,			"ft" },
		{ EDistanceUnit::EDU_Yards,			"yd" },
		{ EDistanceUnit::EDU_Miles,			"mi" },
		{ EDistanceUnit::EDU_Lightyears,	"ly" },
	};

	// These ratios are measured in grams, i.e. 1mg = 0.001g
	inline static const TMap<EWeightUnit, double> WeightRatios =
	{
		{ EWeightUnit::EWU_Micrograms,		0.000001 },
		{ EWeightUnit::EWU_Milligrams,		0.001 },
		{ EWeightUnit::EWU_Grams,			1.0 },
		{ EWeightUnit::EWU_Kilograms,		1000.0 },
		{ EWeightUnit::EWU_MetricTons,		1000000.0 },
		{ EWeightUnit::EWU_Ounces,			28.3495231 },
		{ EWeightUnit::EWU_Pounds,			453.59237 },
		{ EWeightUnit::EWU_Stones,			6350.29317 },
	};

	inline static const TMap<EWeightUnit, FString> WeightSuffix =
	{
		{ EWeightUnit::EWU_Micrograms,		"µg" },
		{ EWeightUnit::EWU_Milligrams,		"mg" },
		{ EWeightUnit::EWU_Grams,			"g" },
		{ EWeightUnit::EWU_Kilograms,		"kg" },
		{ EWeightUnit::EWU_MetricTons,		"t" },
		{ EWeightUnit::EWU_Ounces,			"oz" },
		{ EWeightUnit::EWU_Pounds,			"lb" },
		{ EWeightUnit::EWU_Stones,			"st" },
	};
	
};
