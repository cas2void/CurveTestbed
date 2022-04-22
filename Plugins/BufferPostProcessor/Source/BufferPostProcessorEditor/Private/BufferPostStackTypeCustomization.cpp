// Fill out your copyright notice in the Description page of Project Settings.

#include "BufferPostStackTypeCustomization.h"

#include "DetailLayoutBuilder.h"
#include "PropertyHandle.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"

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
    TSharedRef<IPropertyHandle> EnabledProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FBufferPostStackLayer, bEnabled)).ToSharedRef();
    TSharedRef<IPropertyHandle> TypeProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FBufferPostStackLayer, Type)).ToSharedRef();

    HeaderRow
        .NameContent()
        [
            EnabledProperty->CreatePropertyValueWidget()
        ]
        .ValueContent()
        [
            TypeProperty->CreatePropertyValueWidget()
        ];

    
}

void FBufferPostStackLayerTypeCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
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
        TSharedRef<IPropertyHandle> TypeProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FBufferPostStackLayer, Type)).ToSharedRef();
        TSharedRef<IPropertyHandle> PassProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FBufferPostStackLayer, Pass)).ToSharedRef();
        
        TypeProperty->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(
            [Outer, TypeProperty, PassProperty]()
            {
                uint8 TypeValue;
                TypeProperty->GetValue(TypeValue);
                EBufferPostPassType TypeEnum = static_cast<EBufferPostPassType>(TypeValue);

                UClass* PassClass = UBufferPostPass::GetClassFromType(TypeEnum);
                if (PassClass)
                {
                    UBufferPostPass* Pass = NewObject<UBufferPostPass>(Outer, PassClass);
                    FString PassPathName = Pass->GetPathName();
                    PassProperty->SetValueFromFormattedString(PassPathName);
                }
                else
                {
                    PassProperty->SetValueFromFormattedString(FName(NAME_None).ToString());
                }
            }
        ));

        TSharedPtr<IPropertyHandle> PassSettingsProperty = PassProperty->GetChildHandle(FName(TEXT("PassSettings")));
        if (PassSettingsProperty)
        {
            uint32 NumChildren;
            PassSettingsProperty->GetNumChildren(NumChildren);
            for (uint32 Index = 0; Index < NumChildren; Index++)
            {
                TSharedRef<IPropertyHandle> ChildHandle = PassSettingsProperty->GetChildHandle(Index).ToSharedRef();
                ChildBuilder.AddProperty(ChildHandle);
            }
        }
    }
}

#undef LOCTEXT_NAMESPACE