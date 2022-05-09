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
            auto ToRenderMetaballLamda = [MetaballGenerator]()
            {
                MetaballGenerator->Process();
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
        }
    }
}

void FMetaballGeneratorComponentDetailCustomization::OnPostQueueModified(UMetaballGeneratorComponent* MetaballGenerator)
{
    if (MetaballGenerator)
    {
        MetaballGenerator->Process();
    }
}

#undef LOCTEXT_NAMESPACE