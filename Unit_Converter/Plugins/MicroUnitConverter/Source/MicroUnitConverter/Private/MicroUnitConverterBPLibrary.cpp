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
