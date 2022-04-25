// Fill out your copyright notice in the Description page of Project Settings.

#include "BufferPostStackTypeCustomization.h"

#include "DetailLayoutBuilder.h"
#include "PropertyHandle.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "Widgets/SBoxPanel.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Framework/Commands/UIAction.h"
#include "Widgets/Input/SComboButton.h"
#include "Widgets/Text/STextBlock.h"

#include "BufferPostStack.h"

#define LOCTEXT_NAMESPACE "BufferPostStackTypeCustomization"

TSharedRef<IPropertyTypeCustomization> FBufferPostStackSettingsTypeCustomization::MakeInstance()
{
    return MakeShareable(new FBufferPostStackSettingsTypeCustomization);
}

void FBufferPostStackSettingsTypeCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    FMenuBuilder MenuBuilder(true, nullptr);
    {
        UObject* Outer = nullptr;
        TArray<UObject*> OuterObjects;
        PropertyHandle->GetOuterObjects(OuterObjects);
        if (OuterObjects.Num() > 0)
        {
            Outer = OuterObjects[0];
        }

        if (Outer)
        {
            TArray<UClass*> AllPassClasses = UBufferPostPass::GetAllPassClasses();
            for (auto& PassClass : AllPassClasses)
            {
                FText MenuText = UBufferPostPass::GetDisplayName(PassClass);
                FUIAction Action(FExecuteAction::CreateLambda(
                    [MenuText, PassClass, Outer, PropertyHandle]()
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Selected: %s"), *MenuText.ToString());

                        TSharedRef<IPropertyHandle> LayersProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FBufferPostStackSettings, Layers)).ToSharedRef();
                        TSharedPtr<IPropertyHandleArray> LayersArray = LayersProperty->AsArray();
                        LayersArray->AddItem();
                        uint32 NumElements = INDEX_NONE;
                        LayersArray->GetNumElements(NumElements);
                        if (NumElements > 0)
                        {
                            TSharedPtr<IPropertyHandle> PassProperty = LayersArray->GetElement(NumElements - 1)->GetChildHandle(FName(TEXT("Pass")));
                            if (PassProperty)
                            {
                                UBufferPostPass* NewPass = NewObject<UBufferPostPass>(Outer, PassClass);
                                FString PassPath = NewPass->GetPathName();
                                PassProperty->SetValueFromFormattedString(PassPath);
                            }
                        }
                    }
                ));
                MenuBuilder.AddMenuEntry(MenuText, FText(), FSlateIcon(), Action);
            }
        }
    }

    TSharedRef<IPropertyHandle> EnabledProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FBufferPostStackSettings, bEnabled)).ToSharedRef();
    HeaderRow
        .NameContent()
        [
            PropertyHandle->CreatePropertyNameWidget()
        ]
        .ValueContent()
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .AutoWidth()
            [
                EnabledProperty->CreatePropertyValueWidget()
            ]
            + SHorizontalBox::Slot()
            .AutoWidth()
            .Padding(FMargin(10, 0))
            [
                SNew(SComboButton)
                .IsEnabled(TAttribute<bool>::Create(TAttribute<bool>::FGetter::CreateLambda(
                    [EnabledProperty]()
                    {
                        bool EnabledValue;
                        EnabledProperty->GetValue(EnabledValue);
                        return EnabledValue;
                    }
                )))
                .ButtonContent()
                [
                    SNew(STextBlock)
                    .Text(LOCTEXT("PostStackAddLayerButton", "Add Layer..."))
                ]
                .MenuContent()
                [
                    MenuBuilder.MakeWidget()
                ]
            ]
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
    TSharedRef<IPropertyHandle> EnabledProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FBufferPostStackLayer, bEnabled)).ToSharedRef();
    TSharedRef<IPropertyHandle> PassProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FBufferPostStackLayer, Pass)).ToSharedRef();
    UObject* PassObject = nullptr;
    PassProperty->GetValue(PassObject);
    if (PassObject)
    {
        UClass* PassClass = PassObject->GetClass();
        FText Name = UBufferPostPass::GetDisplayName(PassClass);

        HeaderRow
            .NameContent()
            [
                SNew(STextBlock)
                .Text(Name)
            ]
            .ValueContent()
            [
                EnabledProperty->CreatePropertyValueWidget()
            ];
    }
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

#undef LOCTEXT_NAMESPACE