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

            TSharedPtr<IPropertyHandle> ColorRampProperty = DetailBuilder.GetProperty(TEXT("ColorRamp"));
            if (ColorRampProperty)
            {
                static TWeakObjectPtr<UCurveLinearColor> PreviousExternalCurve = MetaballGenerator->GetColorRamp().ExternalCurve;

                auto OnPropertyValueChanged = [MetaballGenerator, this]()
                {
                    // Respond to external curve's OnUpdateCurve delegate.
                    UCurveLinearColor* CurrentExternalCurve = MetaballGenerator->GetColorRamp().ExternalCurve;
                    if (!PreviousExternalCurve.IsValid() || PreviousExternalCurve != CurrentExternalCurve)
                    {
                        if (PreviousExternalCurve.IsValid())
                        {
                            PreviousExternalCurve->OnUpdateCurve.Remove(ExternalCurveDelegateHandle);
                        }

                        if (CurrentExternalCurve)
                        {
                            auto OnExternalCurveValueChanged = [MetaballGenerator, this](UCurveBase* Curve, EPropertyChangeType::Type Type)
                            {
                                UpdateColorRampTexture(MetaballGenerator);
                            };

                            ExternalCurveDelegateHandle = MetaballGenerator->GetColorRamp().ExternalCurve->OnUpdateCurve.AddLambda(OnExternalCurveValueChanged);
                        }

                        PreviousExternalCurve = CurrentExternalCurve;
                    }

                    UpdateColorRampTexture(MetaballGenerator);
                };

                ColorRampProperty->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(OnPropertyValueChanged));
            }
        }
    }
}

void FMetaballGeneratorComponentDetailCustomization::UpdateColorRampTexture(UMetaballGeneratorComponent* MetaballGeneratorComponent)
{
    MetaballGeneratorComponent->UpdateColorRampTexture();
}

#undef LOCTEXT_NAMESPACE