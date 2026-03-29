// Fill out your copyright notice in the Description page of Project Settings.


#include "NodeStyleCustomization.h"

#include "DetailWidgetRow.h"
#include "DetailLayoutBuilder.h"
#include "IDetailChildrenBuilder.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Colors/SColorBlock.h"

TSharedRef<IPropertyTypeCustomization> FNodeStyleCustomization::MakeInstance()
{
    return MakeShareable(new FNodeStyleCustomization());
}

void FNodeStyleCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    TSharedPtr<IPropertyHandle> EnumHandle = StructHandle->GetChildHandle("Type");
    TSharedPtr<IPropertyHandle> ColorHandle = StructHandle->GetChildHandle("Color");

    // Get enum display name
    FText DisplayName;
    {
        uint8 EnumValue = 0;
        EnumHandle->GetValue(EnumValue);

        const UEnum* EnumType = FindObject<UEnum>(ANY_PACKAGE, TEXT("ENodeType"), true);
        DisplayName = EnumType->GetDisplayNameTextByValue(EnumValue);
    }

    HeaderRow
        .NameContent()
        [
            SNew(SHorizontalBox)

            // Enum display name
            + SHorizontalBox::Slot()
                .AutoWidth()
                .VAlign(VAlign_Center)
                [
                    SNew(STextBlock)
                        .Text(DisplayName)
                        .Font(IDetailLayoutBuilder::GetDetailFont())
                ]

            // Color preview
            + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(6.f, 0.f, 0.f, 0.f)
                .VAlign(VAlign_Center)
                [
                    SNew(SBorder)
                        .Padding(1.f)
                        .BorderImage(FAppStyle::GetBrush("BlackBrush"))
                        [
                            SNew(SColorBlock)
                                .Color(this, &FNodeStyleCustomization::GetColorValue, ColorHandle)
                                .Size(FVector2D(12.f, 12.f))
                                .UseSRGB(true)
                        ]
                ]
        ]
        .ValueContent()
        [
            SNullWidget::NullWidget // blank right side
        ];
}

void FNodeStyleCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    TSharedPtr<IPropertyHandle> ColorHandle = StructHandle->GetChildHandle("Color");

    ChildBuilder.AddProperty(ColorHandle.ToSharedRef());
}

FLinearColor FNodeStyleCustomization::GetColorValue(TSharedPtr<IPropertyHandle> ColorHandle) const
{
    FString ColorString;
    if (ColorHandle->GetValueAsFormattedString(ColorString) == FPropertyAccess::Success)
    {
        FLinearColor Out;
        Out.InitFromString(ColorString);
        return Out;
    }

    return FLinearColor::White;
}

TSharedRef<IPropertyTypeCustomization> FNodeStyleArrayCustomization::MakeInstance()
{
    return MakeShareable(new FNodeStyleArrayCustomization());
}

void FNodeStyleArrayCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    HeaderRow
        .NameContent()
        [
            PropertyHandle->CreatePropertyNameWidget()
        ]
        .ValueContent()
        [
            SNullWidget::NullWidget // blank right side
        ];
}

void FNodeStyleArrayCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    TSharedPtr<IPropertyHandle> ItemsHandle = PropertyHandle->GetChildHandle("Items");

    uint32 NumElements = 0;
    ItemsHandle->GetNumChildren(NumElements);

    for (uint32 Index = 0; Index < NumElements; Index++)
    {
        TSharedPtr<IPropertyHandle> ElementHandle = ItemsHandle->GetChildHandle(Index);

        ChildBuilder.AddProperty(ElementHandle.ToSharedRef());
    }
}


