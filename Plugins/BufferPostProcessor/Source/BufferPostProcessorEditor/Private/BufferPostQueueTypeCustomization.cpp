// Fill out your copyright notice in the Description page of Project Settings.

#include "BufferPostQueueTypeCustomization.h"

#include "DetailLayoutBuilder.h"
#include "PropertyHandle.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Text/STextBlock.h"

#include "BufferPostQueue.h"
#include "BufferRampPass.h"

#define LOCTEXT_NAMESPACE "BufferPostQueueTypeCustomization"

TSharedRef<IPropertyTypeCustomization> FBufferPostQueueSettingsTypeCustomization::MakeInstance()
{
    return MakeShareable(new FBufferPostQueueSettingsTypeCustomization);
}

void FBufferPostQueueSettingsTypeCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    PropertyHandle->MarkResetToDefaultCustomized(true);

    TSharedRef<IPropertyHandle> EnabledProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FBufferPostQueueSettings, bEnabled)).ToSharedRef();
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

void FBufferPostQueueSettingsTypeCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    TSharedRef<IPropertyHandle> LayersProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FBufferPostQueueSettings, Layers)).ToSharedRef();
    LayersProperty->MarkResetToDefaultCustomized(true);

    ChildBuilder
        .AddProperty(LayersProperty)
        .IsEnabled(TAttribute<bool>::Create(TAttribute<bool>::FGetter::CreateLambda(
            [PropertyHandle]()
            {
                TSharedRef<IPropertyHandle> EnabledProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FBufferPostQueueSettings, bEnabled)).ToSharedRef();
                bool EnabledValue;
                EnabledProperty->GetValue(EnabledValue);
                return EnabledValue;
            }
    )));
}

TSharedRef<IPropertyTypeCustomization> FBufferPostQueueLayerTypeCustomization::MakeInstance()
{
    return MakeShareable(new FBufferPostQueueLayerTypeCustomization);
}

void FBufferPostQueueLayerTypeCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    static const FString kEmptyPassNameString(TEXT("None"));

    PropertyHandle->MarkResetToDefaultCustomized(true);

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

    TSharedRef<IPropertyHandle> EnabledProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FBufferPostQueueLayer, bEnabled)).ToSharedRef();
    TSharedRef<IPropertyHandle> PassProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FBufferPostQueueLayer, Pass)).ToSharedRef();

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

void FBufferPostQueueLayerTypeCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    TSharedRef<IPropertyHandle> PassProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FBufferPostQueueLayer, Pass)).ToSharedRef();
    TSharedPtr<IPropertyHandle> PassSettingsProperty = PassProperty->GetChildHandle(FName(TEXT("PassSettings")));
    TSharedRef<IPropertyHandle> EnabledProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FBufferPostQueueLayer, bEnabled)).ToSharedRef();
    auto IsEnabledLamda = [EnabledProperty]()
    {
        bool EnabledValue;
        EnabledProperty->GetValue(EnabledValue);
        return EnabledValue;
    };
    
    if (PassSettingsProperty)
    {
        // Enumerate children of `PassSettings`
        ChildBuilder.AddProperty(PassSettingsProperty.ToSharedRef()).IsEnabled(TAttribute<bool>::Create(TAttribute<bool>::FGetter::CreateLambda(IsEnabledLamda)));

        // Enumerate childern of `Pass`, except `PassSettings`
        TSharedPtr<IPropertyHandle> ParentHandle = PassSettingsProperty->GetParentHandle();
        if (ParentHandle)
        {
            uint32 NumChildren;
            ParentHandle->GetNumChildren(NumChildren);
            for (uint32 Index = 0; Index < NumChildren; Index++)
            {
                TSharedPtr<IPropertyHandle> ChildHandle = ParentHandle->GetChildHandle(Index);
                if (ChildHandle && ChildHandle->GetProperty() != PassSettingsProperty->GetProperty())
                {
                    ChildBuilder.AddProperty(ChildHandle.ToSharedRef()).IsEnabled(TAttribute<bool>::Create(TAttribute<bool>::FGetter::CreateLambda(IsEnabledLamda)));
                }
            }
        }
    }
}

TSharedPtr<FString> FBufferPostQueueLayerTypeCustomization::GetPassTypeOption(UClass* PassClass)
{
    TSharedPtr<FString> Result;

    const TSharedPtr<FString>* PassTypePtr = PassTypeClassMap.FindKey(PassClass);
    if (PassTypePtr)
    {
        Result = *PassTypePtr;
    }

    return Result;
}

TSharedRef<IPropertyTypeCustomization> FBufferRampPassSettingsTypeCustomization::MakeInstance()
{
    return MakeShareable(new FBufferRampPassSettingsTypeCustomization);
}

void FBufferRampPassSettingsTypeCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    TArray<const void*> RawData;
    PropertyHandle->AccessRawData(RawData);
    if (RawData.Num() == 1)
    {
        const FBufferRampPassSettings* RampPassSettings = reinterpret_cast<const FBufferRampPassSettings*>(RawData[0]);
        if (RampPassSettings)
        {
            TSharedRef<IPropertyHandle> RampCurveProperty = PropertyHandle->GetChildHandle(FName(GET_MEMBER_NAME_CHECKED(FBufferRampPassSettings, RampCurve))).ToSharedRef();
            RampCurveProperty->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(
                [RampPassSettings]()
                {
                    RampPassSettings->OnRampCurveModified().Broadcast();
                }
            ));
        }
    }
}

void FBufferRampPassSettingsTypeCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    uint32 NumChildren;
    PropertyHandle->GetNumChildren(NumChildren);
    for (uint32 Index = 0; Index < NumChildren; Index++)
    {
        TSharedPtr<IPropertyHandle> ChildHandle = PropertyHandle->GetChildHandle(Index);
        ChildBuilder.AddProperty(ChildHandle.ToSharedRef());
    }
}

#undef LOCTEXT_NAMESPACE
