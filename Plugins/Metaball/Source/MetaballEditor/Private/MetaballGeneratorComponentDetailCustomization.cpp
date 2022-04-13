// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaballGeneratorComponentDetailCustomization.h"

#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "PropertyHandle.h"
#include "Editor.h"
#include "Editor/EditorEngine.h"

#include "MetaballGeneratorComponent.h"
#include "BufferPresentingEditorSubsystem.h"

#define LOCTEXT_NAMESPACE "MetaballGeneratorComponentDetailCustomization"

TSharedRef<IDetailCustomization> FMetaballGeneratorComponentDetailCustomization::MakeInstance()
{
    return MakeShareable(new FMetaballGeneratorComponentDetailCustomization);
}

void FMetaballGeneratorComponentDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
    TArray<TWeakObjectPtr<UObject>> ObjectsBegingCustomized;
    DetailBuilder.GetObjectsBeingCustomized(ObjectsBegingCustomized);
    if (ObjectsBegingCustomized.Num() == 1)
    {
        UMetaballGeneratorComponent* MetaballGenerator = Cast<UMetaballGeneratorComponent>(ObjectsBegingCustomized[0]);
        if (MetaballGenerator)
        {
            TSharedPtr<IPropertyHandle> RenderTargetSizeProperty = DetailBuilder.GetProperty(TEXT("RenderTargetSize"));
            if (RenderTargetSizeProperty)
            {
                // Set Min/Max
                RenderTargetSizeProperty->GetProperty()->SetMetaData(FName(TEXT("UIMin")), FString::FromInt(720));
                RenderTargetSizeProperty->GetProperty()->SetMetaData(FName(TEXT("ClampMin")), FString::FromInt(720));
                RenderTargetSizeProperty->GetProperty()->SetMetaData(FName(TEXT("UIMax")), FString::FromInt(3840));
                RenderTargetSizeProperty->GetProperty()->SetMetaData(FName(TEXT("ClampMax")), FString::FromInt(3840));

                // OnPropertyValueChanged is called after UActorComponet::OnRegister(), where the buffer has been initialized.
                auto OnPropetyValueChanged = [MetaballGenerator]()
                {
                    UBufferPresentingEditorSubsystem* EditorSubsystem = GEditor->GetEditorSubsystem<UBufferPresentingEditorSubsystem>();
                    if (EditorSubsystem)
                    {
                        EditorSubsystem->Present(MetaballGenerator->GetBuffer());
                    }
                };

                RenderTargetSizeProperty->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(OnPropetyValueChanged));
                RenderTargetSizeProperty->SetOnChildPropertyValueChanged(FSimpleDelegate::CreateLambda(OnPropetyValueChanged));
            }

            auto ToRenderMetaballLamda = [MetaballGenerator]()
            {
                MetaballGenerator->RenderMetbaball();
            };

            TSharedPtr<IPropertyHandle> Point0Property = DetailBuilder.GetProperty(TEXT("Point0"));
            if (Point0Property)
            {
                // Set Min/Max
                Point0Property->GetProperty()->SetMetaData(FName(TEXT("UIMin")), TEXT("0.0f"));
                Point0Property->GetProperty()->SetMetaData(FName(TEXT("ClampMin")), TEXT("0.0f"));
                Point0Property->GetProperty()->SetMetaData(FName(TEXT("UIMax")), TEXT("1.0f"));
                Point0Property->GetProperty()->SetMetaData(FName(TEXT("ClampMax")), TEXT("1.0f"));

                Point0Property->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(ToRenderMetaballLamda));
                Point0Property->SetOnChildPropertyValueChanged(FSimpleDelegate::CreateLambda(ToRenderMetaballLamda));
            }

            TSharedPtr<IPropertyHandle> Point1Property = DetailBuilder.GetProperty(TEXT("Point1"));
            if (Point1Property)
            {
                // Set Min/Max
                Point1Property->GetProperty()->SetMetaData(FName(TEXT("UIMin")), TEXT("0.0f"));
                Point1Property->GetProperty()->SetMetaData(FName(TEXT("ClampMin")), TEXT("0.0f"));
                Point1Property->GetProperty()->SetMetaData(FName(TEXT("UIMax")), TEXT("1.0f"));
                Point1Property->GetProperty()->SetMetaData(FName(TEXT("ClampMax")), TEXT("1.0f"));

                Point1Property->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(ToRenderMetaballLamda));
                Point1Property->SetOnChildPropertyValueChanged(FSimpleDelegate::CreateLambda(ToRenderMetaballLamda));
            }

            TSharedPtr<IPropertyHandle> Point2Property = DetailBuilder.GetProperty(TEXT("Point2"));
            if (Point2Property)
            {
                // Set Min/Max
                Point2Property->GetProperty()->SetMetaData(FName(TEXT("UIMin")), TEXT("0.0f"));
                Point2Property->GetProperty()->SetMetaData(FName(TEXT("ClampMin")), TEXT("0.0f"));
                Point2Property->GetProperty()->SetMetaData(FName(TEXT("UIMax")), TEXT("1.0f"));
                Point2Property->GetProperty()->SetMetaData(FName(TEXT("ClampMax")), TEXT("1.0f"));

                Point2Property->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(ToRenderMetaballLamda));
                Point2Property->SetOnChildPropertyValueChanged(FSimpleDelegate::CreateLambda(ToRenderMetaballLamda));
            }

            TSharedPtr<IPropertyHandle> MinIntensityProperty = DetailBuilder.GetProperty(TEXT("MinIntensity"));
            if (MinIntensityProperty)
            {
                // Set Min/Max
                MinIntensityProperty->GetProperty()->SetMetaData(FName(TEXT("UIMin")), TEXT("0.0f"));
                MinIntensityProperty->GetProperty()->SetMetaData(FName(TEXT("ClampMin")), TEXT("0.0f"));
                MinIntensityProperty->GetProperty()->SetMetaData(FName(TEXT("UIMax")), TEXT("10.0f"));
                MinIntensityProperty->GetProperty()->SetMetaData(FName(TEXT("ClampMax")), TEXT("10.0f"));

                MinIntensityProperty->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(ToRenderMetaballLamda));
            }

            TSharedPtr<IPropertyHandle> MaxIntensityProperty = DetailBuilder.GetProperty(TEXT("MaxIntensity"));
            if (MaxIntensityProperty)
            {
                // Set Min/Max
                MaxIntensityProperty->GetProperty()->SetMetaData(FName(TEXT("UIMin")), TEXT("1.0f"));
                MaxIntensityProperty->GetProperty()->SetMetaData(FName(TEXT("ClampMin")), TEXT("1.0f"));
                MaxIntensityProperty->GetProperty()->SetMetaData(FName(TEXT("UIMax")), TEXT("1000.0f"));
                MaxIntensityProperty->GetProperty()->SetMetaData(FName(TEXT("ClampMax")), TEXT("1000.0f"));

                MaxIntensityProperty->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(ToRenderMetaballLamda));
            }

            TSharedPtr<IPropertyHandle> ColorRampProperty = DetailBuilder.GetProperty(TEXT("ColorRamp"));
            if (ColorRampProperty)
            {
                auto OnPropertyValueChanged = [MetaballGenerator, this]()
                {
                    MetaballGenerator->UpdateColorRampTexture();
                    MetaballGenerator->RenderMetbaball();
                };

                ColorRampProperty->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(OnPropertyValueChanged));
            }
        }
    }
}

#undef LOCTEXT_NAMESPACE