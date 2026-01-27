#include "MicroUnitConverterBPLibrary.h"
#include "MicroUnitConverter.h"

UMicroUnitConverterBPLibrary::UMicroUnitConverterBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

double UMicroUnitConverterBPLibrary::ConvertUnit_Distance(double InValue, EDistanceUnit InUnit, EDistanceUnit OutUnit)
{
	double InRatio = DistanceRatios[InUnit];
	double OutRatio = DistanceRatios[OutUnit];

	return InValue * (InRatio / OutRatio);
}

void UMicroUnitConverterBPLibrary::PrintConversion_Distance(double InValue, EDistanceUnit InUnit, EDistanceUnit OutUnit, FPrintOptions PrintOptions)
{
	double ConvertedValue = ConvertUnit_Distance(InValue, InUnit, OutUnit);
	FString OutUnitSuffix = DistanceSuffix[OutUnit];

	PrintImpl(ConvertedValue, OutUnitSuffix, PrintOptions);
}

double UMicroUnitConverterBPLibrary::ConvertUnit_Weight(double InValue, EWeightUnit InUnit, EWeightUnit OutUnit)
{
	double InRatio = WeightRatios[InUnit];
	double OutRatio = WeightRatios[OutUnit];

	return InValue * (InRatio / OutRatio);
}

void UMicroUnitConverterBPLibrary::PrintConversion_Weight(double InValue, EWeightUnit InUnit, EWeightUnit OutUnit, FPrintOptions PrintOptions)
{
	double ConvertedValue = ConvertUnit_Weight(InValue, InUnit, OutUnit);
	FString OutUnitSuffix = WeightSuffix[OutUnit];

	PrintImpl(ConvertedValue, OutUnitSuffix, PrintOptions);
}

double UMicroUnitConverterBPLibrary::ConvertUnit_Time(double InValue, ETimeUnit InUnit, ETimeUnit OutUnit)
{
	double InRatio = TimeRatios[InUnit];
	double OutRatio = TimeRatios[OutUnit];

	return InValue * (InRatio / OutRatio);
}

void UMicroUnitConverterBPLibrary::PrintConversion_Time(double InValue, ETimeUnit InUnit, ETimeUnit OutUnit, FPrintOptions PrintOptions)
{
	double ConvertedValue = ConvertUnit_Time(InValue, InUnit, OutUnit);
	FString OutUnitSuffix = TimeSuffix[OutUnit];

	PrintImpl(ConvertedValue, OutUnitSuffix, PrintOptions);
}

double UMicroUnitConverterBPLibrary::ConvertUnit_Binary(double InValue, EBinaryUnit InUnit, EBinaryUnit OutUnit)
{
	double InRatio = BinaryRatios[InUnit];
	double OutRatio = BinaryRatios[OutUnit];

	return InValue * (InRatio / OutRatio);
}

void UMicroUnitConverterBPLibrary::PrintConversion_Binary(double InValue, EBinaryUnit InUnit, EBinaryUnit OutUnit, FPrintOptions PrintOptions)
{
	double ConvertedValue = ConvertUnit_Binary(InValue, InUnit, OutUnit);
	FString OutUnitSuffix = BinarySuffix[OutUnit];

	PrintImpl(ConvertedValue, OutUnitSuffix, PrintOptions);
}

double UMicroUnitConverterBPLibrary::ConvertUnit_Temperature(double InValue, ETemperatureUnit InUnit, ETemperatureUnit OutUnit)
{
	auto InInfo = TemperatureInfos[InUnit];
	auto OutInfo = TemperatureInfos[OutUnit];

	double ToF = (InValue * InInfo.Scale) + InInfo.Offset;
	return (ToF - OutInfo.Offset) / OutInfo.Scale;
}

void UMicroUnitConverterBPLibrary::PrintConversion_Temperature(double InValue, ETemperatureUnit InUnit, ETemperatureUnit OutUnit, FPrintOptions PrintOptions)
{
	double ConvertedValue = ConvertUnit_Temperature(InValue, InUnit, OutUnit);
	FString OutUnitSuffix = TemperatureSuffix[OutUnit];

	PrintImpl(ConvertedValue, OutUnitSuffix, PrintOptions);
}

double UMicroUnitConverterBPLibrary::ConvertUnit_Frequency(double InValue, EFrequencyUnit InUnit, EFrequencyUnit OutUnit)
{
	double InRatio = FrequencyRatios[InUnit];
	double OutRatio = FrequencyRatios[OutUnit];

	return InValue * (InRatio / OutRatio);
}

void UMicroUnitConverterBPLibrary::PrintConversion_Frequency(double InValue, EFrequencyUnit InUnit, EFrequencyUnit OutUnit, FPrintOptions PrintOptions)
{
	double ConvertedValue = ConvertUnit_Frequency(InValue, InUnit, OutUnit);
	FString OutUnitSuffix = FrequencySuffix[OutUnit];

	PrintImpl(ConvertedValue, OutUnitSuffix, PrintOptions);
}

double UMicroUnitConverterBPLibrary::ConvertUnit_Force(double InValue, EForceUnit InUnit, EForceUnit OutUnit)
{
	double InRatio = ForceRatios[InUnit];
	double OutRatio = ForceRatios[OutUnit];

	return InValue * (InRatio / OutRatio);
}

void UMicroUnitConverterBPLibrary::PrintConversion_Force(double InValue, EForceUnit InUnit, EForceUnit OutUnit, FPrintOptions PrintOptions)
{
	double ConvertedValue = ConvertUnit_Force(InValue, InUnit, OutUnit);
	FString OutUnitSuffix = ForceSuffix[OutUnit];

	PrintImpl(ConvertedValue, OutUnitSuffix, PrintOptions);
}

double UMicroUnitConverterBPLibrary::ConvertUnit_Pressure(double InValue, EPressureUnit InUnit, EPressureUnit OutUnit)
{
	double InRatio = PressureRatios[InUnit];
	double OutRatio = PressureRatios[OutUnit];

	return InValue * (InRatio / OutRatio);
}

void UMicroUnitConverterBPLibrary::PrintConversion_Pressure(double InValue, EPressureUnit InUnit, EPressureUnit OutUnit, FPrintOptions PrintOptions)
{
	double ConvertedValue = ConvertUnit_Pressure(InValue, InUnit, OutUnit);
	FString OutUnitSuffix = PressureSuffix[OutUnit];

	PrintImpl(ConvertedValue, OutUnitSuffix, PrintOptions);
}

double UMicroUnitConverterBPLibrary::ConvertUnit_Density(double InValue, EDensityUnit InUnit, EDensityUnit OutUnit)
{
	double InRatio = DensityRatios[InUnit];
	double OutRatio = DensityRatios[OutUnit];

	return InValue * (InRatio / OutRatio);
}

void UMicroUnitConverterBPLibrary::PrintConversion_Density(double InValue, EDensityUnit InUnit, EDensityUnit OutUnit, FPrintOptions PrintOptions)
{
	double ConvertedValue = ConvertUnit_Density(InValue, InUnit, OutUnit);
	FString OutUnitSuffix = DensitySuffix[OutUnit];

	PrintImpl(ConvertedValue, OutUnitSuffix, PrintOptions);
}

double UMicroUnitConverterBPLibrary::ConvertUnit_Speed(double InValue, ESpeedUnit InUnit, ESpeedUnit OutUnit)
{
	double InRatio = SpeedRatios[InUnit];
	double OutRatio = SpeedRatios[OutUnit];

	return InValue * (InRatio / OutRatio);
}

void UMicroUnitConverterBPLibrary::PrintConversion_Speed(double InValue, ESpeedUnit InUnit, ESpeedUnit OutUnit, FPrintOptions PrintOptions)
{
	double ConvertedValue = ConvertUnit_Speed(InValue, InUnit, OutUnit);
	FString OutUnitSuffix = SpeedSuffix[OutUnit];

	PrintImpl(ConvertedValue, OutUnitSuffix, PrintOptions);
}

void UMicroUnitConverterBPLibrary::PrintImpl(double ConvertedValue, FString UnitSuffix, FPrintOptions PrintOptions)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FNumberFormattingOptions Opts = {};
	Opts.MinimumFractionalDigits = PrintOptions.MinDecimalPlaces;
	Opts.MaximumFractionalDigits = PrintOptions.MaxDecimalPlaces;

	FText OutTextValue = FText::AsNumber(ConvertedValue, &Opts);

	if (PrintOptions.bPrintToScreen && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Format(TEXT("{0}{1}"), { OutTextValue.ToString(), UnitSuffix }));
	}

	if (PrintOptions.bPrintToLog)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s%s"), *OutTextValue.ToString(), *UnitSuffix);
	}
#endif
}
