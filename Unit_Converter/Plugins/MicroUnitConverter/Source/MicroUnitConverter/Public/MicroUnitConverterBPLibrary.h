#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "MicroUnitConverterBPLibrary.generated.h"

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


UCLASS()
class UMicroUnitConverterBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	// Converts value from one distance unit to another using the appropriate ratio
	UFUNCTION(BlueprintPure, Category = "Micro Unit Converter", meta = (DisplayName = "Convert Unit - Distance"))
	static double ConvertUnit_Distance(double InValue, EDistanceUnit InUnit, EDistanceUnit OutUnit);

	// Prints the converted distance value to screen / output log
	UFUNCTION(BlueprintCallable, Category = "Micro Unit Converter", meta = (DisplayName = "Print Conversion - Distance", DevelopmentOnly, AdvancedDisplay = 3))
	static void PrintConversion_Distance(double InValue, EDistanceUnit InUnit, EDistanceUnit OutUnit, int32 MinDecimalPlaces = 0, int32 MaxDecimalPlaces = 6, bool bPrintToScreen = true, bool bPrintToLog = true);

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
	
};
