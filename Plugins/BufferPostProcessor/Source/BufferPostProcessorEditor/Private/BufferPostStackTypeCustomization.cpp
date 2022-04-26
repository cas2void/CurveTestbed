// Fill out your copyright notice in the Description page of Project Settings.

#include "BufferPostStackTypeCustomization.h"

#include "DetailLayoutBuilder.h"
#include "PropertyHandle.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Text/STextBlock.h"

#include "BufferPostStack.h"

#define LOCTEXT_NAMESPACE "BufferPostStackTypeCustomization"

TSharedRef<IPropertyTypeCustomization> FBufferPostStackSettingsTypeCustomization::MakeInstance()
{
    return MakeShareable(new FBufferPostStackSettingsTypeCustomization);
}

void FBufferPostStackSettingsTypeCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    TSharedRef<IPropertyHandle> EnabledProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FBufferPostStackSettings, bEnabled)).ToSharedRef();
    HeaderRow
        .NameContent()
        [
            PropertyHandle->CreatePropertyNameWidget()
        ]
        .ValueContent()
        [
            EnabledProperty->CreatePropertyValueWidget()
        ];
}

void FBufferPostStackSettingsTypeCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    TSharedRef<IPropertyHandle> LayersProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FBufferPostStackSettings, Layers)).ToSharedRef();
    ChildBuilder
        .AddProperty(LayersProperty)
        .IsEnabled(TAttribute<bool>::Create(TAttribute<bool>::FGetter::CreateLambda(
            [PropertyHandle]()
            {
                TSharedRef<IPropertyHandle> EnabledProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FBufferPostStackSettings, bEnabled)).ToSharedRef();
                bool EnabledValue;
                EnabledProperty->GetValue(EnabledValue);
                return EnabledValue;
            }
    )));
}

TSharedRef<IPropertyTypeCustomization> FBufferPostStackLayerTypeCustomization::MakeInstance()
{
    return MakeShareable(new FBufferPostStackLayerTypeCustomization);
}

void FBufferPostStackLayerTypeCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    static const FString kEmptyPassNameString(TEXT("None"));

    // Initialize menu options.
    // `PassTypeClassMap` and `PassTypeOptions` are defined as class member, as they need to be accessed in slate delegates
    PassTypeClassMap.Add(MakeShareable(new FString(kEmptyPassNameString)), nullptr);
    TArray<UClass*> AllPassClasses = UBufferPostPass::GetAllPassClasses();
    for (auto& PassClass : AllPassClasses)
    {
        FString MenuString = UBufferPostPass::GetDisplayName(PassClass);
        PassTypeClassMap.Add(MakeShareable(new FString(MenuString)), PassClass);
    }
    PassTypeClassMap.GetKeys(PassTypeOptions);

    // `Outer` is used in creating `UBufferPostPass` object.
    UObject* Outer = nullptr;
    TArray<UObject*> OuterObjects;
    PropertyHandle->GetOuterObjects(OuterObjects);
    if (OuterObjects.Num() > 0)
    {
        Outer = OuterObjects[0];
    }

    TSharedRef<IPropertyHandle> EnabledProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FBufferPostStackLayer, bEnabled)).ToSharedRef();
    TSharedRef<IPropertyHandle> PassProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FBufferPostStackLayer, Pass)).ToSharedRef();

    // Current `Pass` type class
    UClass* PassClass = nullptr;
    UObject* PassObject = nullptr;
    PassProperty->GetValue(PassObject);
    if (PassObject)
    {
        PassClass = PassObject->GetClass();
    }

    // Get combo box label from current `PassObject`
    FText PassTypeText = FText::FromString(kEmptyPassNameString);
    if (PassClass)
    {
        PassTypeText = FText::FromString(UBufferPostPass::GetDisplayName(PassClass));
    }

    // Set initial selected item from current `PassObject`
    TSharedPtr<FString> SelectedPassType = GetPassTypeOption(PassClass);

    HeaderRow
        .NameContent()
        [
            SNew(SComboBox<TSharedPtr<FString>>)
            .IsEnabled(TAttribute<bool>::Create(TAttribute<bool>::FGetter::CreateLambda(
                [EnabledProperty]()
                {
                    bool EnabledValue;
                    EnabledProperty->GetValue(EnabledValue);
                    return EnabledValue;
                }
            )))
            .OptionsSource(&PassTypeOptions)
            .OnGenerateWidget(SComboBox<TSharedPtr<FString>>::FOnGenerateWidget::CreateLambda(
                [](TSharedPtr<FString> StringItem)
                {
                    return SNew(STextBlock).Text(FText::FromString(*StringItem));
                }
            ))
            .OnSelectionChanged(SComboBox<TSharedPtr<FString>>::FOnSelectionChanged::CreateLambda(
                [Outer, PassProperty, this](TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Selected: %s"), NewSelection ? *(*NewSelection) : TEXT(" Empty "));

                    if (Outer)
                    {
                        UClass** PassClassPtr = PassTypeClassMap.Find(NewSelection);
                        if (PassClassPtr)
                        {
                            if (*PassClassPtr)
                            {
                                UBufferPostPass* NewPass = NewObject<UBufferPostPass>(Outer, *PassClassPtr);
                                FString PassPath = NewPass->GetPathName();
                                PassProperty->SetValueFromFormattedString(PassPath);
                            }
                            else
                            {
                                PassProperty->SetValueFromFormattedString(FName(NAME_None).ToString());
                            }
                        }
                    }
                }
            ))
            .InitiallySelectedItem(SelectedPassType)
            [
                SNew(STextBlock)
                .Text(PassTypeText)
            ]
        ]
        .ValueContent()
        [
            EnabledProperty->CreatePropertyValueWidget()
        ];
}

void FBufferPostStackLayerTypeCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    TSharedRef<IPropertyHandle> PassProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FBufferPostStackLayer, Pass)).ToSharedRef();
    TSharedPtr<IPropertyHandle> PassSettingsProperty = PassProperty->GetChildHandle(FName(TEXT("PassSettings")));
    if (PassSettingsProperty)
    {
        TSharedRef<IPropertyHandle> EnabledProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FBufferPostStackLayer, bEnabled)).ToSharedRef();
        auto IsEnabledLamda = [EnabledProperty]()
        {
            bool EnabledValue;
            EnabledProperty->GetValue(EnabledValue);
            return EnabledValue;
        };

        uint32 NumChildren;
        PassSettingsProperty->GetNumChildren(NumChildren);
        for (uint32 Index = 0; Index < NumChildren; Index++)
        {
            TSharedRef<IPropertyHandle> ChildHandle = PassSettingsProperty->GetChildHandle(Index).ToSharedRef();
            ChildBuilder.AddProperty(ChildHandle).IsEnabled(TAttribute<bool>::Create(TAttribute<bool>::FGetter::CreateLambda(IsEnabledLamda)));
        }
    }
}

TSharedPtr<FString> FBufferPostStackLayerTypeCustomization::GetPassTypeOption(UClass* PassClass)
{
    TSharedPtr<FString> Result;

    const TSharedPtr<FString>* PassTypePtr = PassTypeClassMap.FindKey(PassClass);
    if (PassTypePtr)
    {
        Result = *PassTypePtr;
    }

    return Result;
}

#undef LOCTEXT_NAMESPACE