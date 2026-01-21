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

UENUM(BlueprintType)
enum class ETimeUnit : uint8
{
	ETU_Nanoseconds		UMETA(DisplayName = "Nanoseconds"),
	ETU_Microseconds	UMETA(DisplayName = "Microseconds"),
	ETU_Milliseconds	UMETA(DisplayName = "Milliseconds"),
	ETU_Seconds			UMETA(DisplayName = "Seconds"),
	ETU_Minutes			UMETA(DisplayName = "Minutes"),
	ETU_Hours			UMETA(DisplayName = "Hours"),
	ETU_Days			UMETA(DisplayName = "Days"),
	ETU_Months			UMETA(DisplayName = "Months"),
	ETU_Years			UMETA(DisplayName = "Years"),
};

UENUM(BlueprintType)
enum class EBinaryUnit : uint8
{
	EBU_Bits			UMETA(DisplayName = "Bits"),
	EBU_Bytes			UMETA(DisplayName = "Bytes"),
	EBU_Kilobits		UMETA(DisplayName = "Kilobits"),
	EBU_Kibibits		UMETA(DisplayName = "Kibibits"),
	EBU_Kilobytes		UMETA(DisplayName = "Kilobytes"),
	EBU_Kibibytes		UMETA(DisplayName = "Kibibytes"),
	EBU_Megabits		UMETA(DisplayName = "Megabits"),
	EBU_Mebibits		UMETA(DisplayName = "Mebibits"),
	EBU_Megabytes		UMETA(DisplayName = "Megabytes"),
	EBU_Mebibytes		UMETA(DisplayName = "Mebibytes"),
	EBU_Gigabits		UMETA(DisplayName = "Gigabits"),
	EBU_Gibibits		UMETA(DisplayName = "Gibibits"),
	EBU_Gigabytes		UMETA(DisplayName = "Gigabytes"),
	EBU_Gibibytes		UMETA(DisplayName = "Gibibytes"),
	EBU_Terabits		UMETA(DisplayName = "Terabits"),
	EBU_Tebibits		UMETA(DisplayName = "Tebibits"),
	EBU_Terabytes		UMETA(DisplayName = "Terabytes"),
	EBU_Tebibytes		UMETA(DisplayName = "Tebibytes"),
};

UENUM(BlueprintType)
enum class ETemperatureUnit : uint8
{
	ETU_Celsius			UMETA(DisplayName = "Celsius"),
	ETU_Fahrenheit		UMETA(DisplayName = "Fahrenheit"),
	ETU_Kelvin			UMETA(DisplayName = "Kelvin"),
	ETU_Rankine			UMETA(DisplayName = "Rankine"),
};

struct FTemperatureConversionInfo
{
	double Scale = 1.0;
	double Offset = 0.0;
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

	// Converts value from one time unit to another using the appropriate ratio
	UFUNCTION(BlueprintPure, Category = "Micro Unit Converter", meta = (DisplayName = "Convert Unit - Time"))
	static double ConvertUnit_Time(double InValue, ETimeUnit InUnit, ETimeUnit OutUnit);

	// Prints the converted time value to screen / output log
	UFUNCTION(BlueprintCallable, Category = "Micro Unit Converter", meta = (DisplayName = "Print Conversion - Time", DevelopmentOnly))
	static void PrintConversion_Time(double InValue, ETimeUnit InUnit, ETimeUnit OutUnit, FPrintOptions PrintOptions);

	// Converts value from one binary unit to another using the appropriate ratio
	UFUNCTION(BlueprintPure, Category = "Micro Unit Converter", meta = (DisplayName = "Convert Unit - Binary"))
	static double ConvertUnit_Binary(double InValue, EBinaryUnit InUnit, EBinaryUnit OutUnit);

	// Prints the converted binary value to screen / output log
	UFUNCTION(BlueprintCallable, Category = "Micro Unit Converter", meta = (DisplayName = "Print Conversion - Binary", DevelopmentOnly))
	static void PrintConversion_Binary(double InValue, EBinaryUnit InUnit, EBinaryUnit OutUnit, FPrintOptions PrintOptions);

	// Converts value from one temperature unit to another using the appropriate ratio
	UFUNCTION(BlueprintPure, Category = "Micro Unit Converter", meta = (DisplayName = "Convert Unit - Temperature"))
	static double ConvertUnit_Temperature(double InValue, ETemperatureUnit InUnit, ETemperatureUnit OutUnit);

	// Prints the converted temperature value to screen / output log
	UFUNCTION(BlueprintCallable, Category = "Micro Unit Converter", meta = (DisplayName = "Print Conversion - Temperature", DevelopmentOnly))
	static void PrintConversion_Temperature(double InValue, ETemperatureUnit InUnit, ETemperatureUnit OutUnit, FPrintOptions PrintOptions);

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

	// These ratios are measured in seconds, i.e. 1ms = 0.001s
	inline static const TMap<ETimeUnit, double> TimeRatios =
	{
		{ ETimeUnit::ETU_Nanoseconds,		0.000000001 },
		{ ETimeUnit::ETU_Microseconds,		0.000001 },
		{ ETimeUnit::ETU_Milliseconds,		0.001 },
		{ ETimeUnit::ETU_Seconds,			1.0 },
		{ ETimeUnit::ETU_Minutes,			60.0 },
		{ ETimeUnit::ETU_Hours,				3600.0 },
		{ ETimeUnit::ETU_Days,				86400.0 },
		{ ETimeUnit::ETU_Months,			2628000.0 },	// ~30.4167 days (365 days/12 months)
		{ ETimeUnit::ETU_Years,				31536000.0 },	// 365 days
	};

	inline static const TMap<ETimeUnit, FString> TimeSuffix =
	{
		{ ETimeUnit::ETU_Nanoseconds,		"ns" },
		{ ETimeUnit::ETU_Microseconds,		"µs" },
		{ ETimeUnit::ETU_Milliseconds,		"ms" },
		{ ETimeUnit::ETU_Seconds,			"s" },
		{ ETimeUnit::ETU_Minutes,			"min" },
		{ ETimeUnit::ETU_Hours,				"h" },
		{ ETimeUnit::ETU_Days,				"d" },
		{ ETimeUnit::ETU_Months,			"mo" },
		{ ETimeUnit::ETU_Years,				"yr" },
	};

	// These ratios are measured in kilobytes, i.e. 1MB = 1000KB
	inline static const TMap<EBinaryUnit, double> BinaryRatios =
	{
		{ EBinaryUnit::EBU_Bits,				0.000125 },
		{ EBinaryUnit::EBU_Bytes,				0.001 },
		{ EBinaryUnit::EBU_Kilobits,			0.125 },
		{ EBinaryUnit::EBU_Kibibits,			0.128 },
		{ EBinaryUnit::EBU_Kilobytes,			1.0 },
		{ EBinaryUnit::EBU_Kibibytes,			1.024 },
		{ EBinaryUnit::EBU_Megabits,			125.0 },
		{ EBinaryUnit::EBU_Mebibits,			131.072 },
		{ EBinaryUnit::EBU_Megabytes,			1000.0 },
		{ EBinaryUnit::EBU_Mebibytes,			1048.576 },
		{ EBinaryUnit::EBU_Gigabits,			125000.0 },
		{ EBinaryUnit::EBU_Gibibits,			134217.728 },
		{ EBinaryUnit::EBU_Gigabytes,			1000000.0 },
		{ EBinaryUnit::EBU_Gibibytes,			1073741.824 },
		{ EBinaryUnit::EBU_Terabits,			125000000.0 },
		{ EBinaryUnit::EBU_Tebibits,			137438953.472 },
		{ EBinaryUnit::EBU_Terabytes,			1000000000.0 },
		{ EBinaryUnit::EBU_Tebibytes,			1099511627.776 },
	};

	inline static const TMap<EBinaryUnit, FString> BinarySuffix =
	{
		{ EBinaryUnit::EBU_Bits,				"b" },
		{ EBinaryUnit::EBU_Bytes,				"B" },
		{ EBinaryUnit::EBU_Kilobits,			"Kb" },
		{ EBinaryUnit::EBU_Kibibits,			"Kib" },
		{ EBinaryUnit::EBU_Kilobytes,			"KB" },
		{ EBinaryUnit::EBU_Kibibytes,			"KiB" },
		{ EBinaryUnit::EBU_Megabits,			"Mb" },
		{ EBinaryUnit::EBU_Mebibits,			"Mib" },
		{ EBinaryUnit::EBU_Megabytes,			"MB" },
		{ EBinaryUnit::EBU_Mebibytes,			"MiB" },
		{ EBinaryUnit::EBU_Gigabits,			"Gb" },
		{ EBinaryUnit::EBU_Gibibits,			"Gib" },
		{ EBinaryUnit::EBU_Gigabytes,			"GB" },
		{ EBinaryUnit::EBU_Gibibytes,			"GiB" },
		{ EBinaryUnit::EBU_Terabits,			"Tb" },
		{ EBinaryUnit::EBU_Tebibits,			"Tib" },
		{ EBinaryUnit::EBU_Terabytes,			"TB" },
		{ EBinaryUnit::EBU_Tebibytes,			"TiB" },
	};

	// These ratios are measured in fahrenheit, i.e. 100°C = 212°F
	inline static const TMap<ETemperatureUnit, FTemperatureConversionInfo> TemperatureInfos =
	{
		{ ETemperatureUnit::ETU_Celsius,		{ 1.8, 32.0 } },
		{ ETemperatureUnit::ETU_Fahrenheit,		{ 1.0, 0.0 } },
		{ ETemperatureUnit::ETU_Kelvin,			{ 1.8, -459.67 } },
		{ ETemperatureUnit::ETU_Rankine,		{ 1.0, -459.67 } },
	};

	inline static const TMap<ETemperatureUnit, FString> TemperatureSuffix =
	{
		{ ETemperatureUnit::ETU_Celsius,		"*C" },
		{ ETemperatureUnit::ETU_Fahrenheit,		"*F" },
		{ ETemperatureUnit::ETU_Kelvin,			"*K" },
		{ ETemperatureUnit::ETU_Rankine,		"*R" },
	};
	
};
