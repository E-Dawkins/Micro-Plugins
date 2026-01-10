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

void UMicroUnitConverterBPLibrary::PrintConversion_Distance(double InValue, EDistanceUnit InUnit, EDistanceUnit OutUnit, int32 MinDecimalPlaces, int32 MaxDecimalPlaces, bool bPrintToScreen, bool bPrintToLog)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	FNumberFormattingOptions Opts = {};
	Opts.MinimumFractionalDigits = MinDecimalPlaces;
	Opts.MaximumFractionalDigits = MaxDecimalPlaces;

	double ConvertedValue = ConvertUnit_Distance(InValue, InUnit, OutUnit);
	FText OutTextValue = FText::AsNumber(ConvertedValue, &Opts);

	FString OutUnitSuffix = DistanceSuffix[OutUnit];

	if (bPrintToScreen && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Format(TEXT("{0}{1}"), { OutTextValue.ToString(), OutUnitSuffix }));
	}

	if (bPrintToLog)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s%s"), *OutTextValue.ToString(), *OutUnitSuffix);
	}
#endif
}
