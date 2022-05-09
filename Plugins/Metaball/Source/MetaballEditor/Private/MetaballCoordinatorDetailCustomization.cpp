// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaballCoordinatorDetailCustomization.h"

#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "Editor.h"
#include "Editor/EditorEngine.h"

#include "BufferPresentingGameSubsystem.h"
#include "BufferPresentingEditorSubsystem.h"
#include "MetaballCoordinator.h"

#define LOCTEXT_NAMESPACE "MetaballCoordinatorDetailCustomization"

TSharedRef<IDetailCustomization> FMetaballCoordinatorDetailCustomization::MakeInstance()
{
    return MakeShareable(new FMetaballCoordinatorDetailCustomization);
}

void FMetaballCoordinatorDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
    ObjectsBeingCustomized.Empty();
    DetailBuilder.GetObjectsBeingCustomized(ObjectsBeingCustomized);

    if (ObjectsBeingCustomized.Num() == 1)
    {
        IDetailCategoryBuilder& CategoryBuilder = DetailBuilder.EditCategory(TEXT("BufferPresenting"), FText::GetEmpty(), ECategoryPriority::TypeSpecific);
        CategoryBuilder.AddCustomRow(LOCTEXT("BufferPresentingFiltering", "Buffer Presenting"))
            .WholeRowContent()
            [
                SNew(SButton)
                .HAlign(HAlign_Center)
                .Text(LOCTEXT("PresentText", "Present Metaball"))
                .OnClicked(this, &FMetaballCoordinatorDetailCustomization::OnMetaballPresent)
            ];

        CategoryBuilder.AddCustomRow(LOCTEXT("BufferPresentingFiltering", "Buffer Presenting"))
            .WholeRowContent()
            [
                SNew(SButton)
                .HAlign(HAlign_Center)
                .Text(LOCTEXT("ShutdownText", "Shutdown"))
                .OnClicked(this, &FMetaballCoordinatorDetailCustomization::OnShutdown)
            ];

        TSharedPtr<IPropertyHandle> RenderTargetSizeProperty = DetailBuilder.GetProperty(TEXT("RenderTargetSize"));
        if (RenderTargetSizeProperty)
        {
            // Set Min/Max
            RenderTargetSizeProperty->GetProperty()->SetMetaData(FName(TEXT("UIMin")), FString::FromInt(720));
            RenderTargetSizeProperty->GetProperty()->SetMetaData(FName(TEXT("ClampMin")), FString::FromInt(720));
            RenderTargetSizeProperty->GetProperty()->SetMetaData(FName(TEXT("UIMax")), FString::FromInt(3840));
            RenderTargetSizeProperty->GetProperty()->SetMetaData(FName(TEXT("ClampMax")), FString::FromInt(3840));

            AMetaballCoordinator* MetaballCoordinator = Cast<AMetaballCoordinator>(ObjectsBeingCustomized[0].Get());
            if (MetaballCoordinator)
            {
                // OnPropertyValueChanged is called after all child components' UActorComponet::OnRegister(), where buffers of components has been initialized.
                auto OnPropetyValueChanged = [MetaballCoordinator]()
                {
                    MetaballCoordinator->GetMetaballGeneratorComponent()->SetSize(MetaballCoordinator->GetRenderTargetSize());
                    UBufferPresentingEditorSubsystem* BufferPresentingSubsystem = GEditor->GetEditorSubsystem<UBufferPresentingEditorSubsystem>();
                    if (BufferPresentingSubsystem && BufferPresentingSubsystem->IsPresenting())
                    {
                        BufferPresentingSubsystem->Present(MetaballCoordinator->GetMetaballGeneratorComponent()->GetOutput(), MetaballCoordinator->GetMetaballGeneratorComponent()->IsOutputMonochrome());
                    }
                };

                RenderTargetSizeProperty->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda(OnPropetyValueChanged));
                RenderTargetSizeProperty->SetOnChildPropertyValueChanged(FSimpleDelegate::CreateLambda(OnPropetyValueChanged));
            }

            CategoryBuilder.AddProperty(RenderTargetSizeProperty);
        }
    }
}

FReply FMetaballCoordinatorDetailCustomization::OnMetaballPresent()
{
    if (ObjectsBeingCustomized.Num() == 1)
    {
        AMetaballCoordinator* MetaballCoordinator = Cast<AMetaballCoordinator>(ObjectsBeingCustomized[0].Get());
        if (MetaballCoordinator)
        {
            UBufferPresentingEditorSubsystem* BufferPresentingSubsystem = GEditor->GetEditorSubsystem<UBufferPresentingEditorSubsystem>();
            if (BufferPresentingSubsystem)
            {
                BufferPresentingSubsystem->Present(MetaballCoordinator->GetMetaballGeneratorComponent()->GetOutput(), MetaballCoordinator->GetMetaballGeneratorComponent()->IsOutputMonochrome());
            }
        }
    }

    return FReply::Handled();
}

FReply FMetaballCoordinatorDetailCustomization::OnShutdown()
{
    if (ObjectsBeingCustomized.Num() == 1)
    {
        UBufferPresentingEditorSubsystem* BufferPresentingSubsystem = GEditor->GetEditorSubsystem<UBufferPresentingEditorSubsystem>();
        if (BufferPresentingSubsystem)
        {
            BufferPresentingSubsystem->Shutdown();
        }
    }

    return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE