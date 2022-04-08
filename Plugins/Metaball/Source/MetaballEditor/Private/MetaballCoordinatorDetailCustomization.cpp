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

    IDetailCategoryBuilder& CategoryBuilder = DetailBuilder.EditCategory(TEXT("BufferPresenting"), FText::GetEmpty(), ECategoryPriority::TypeSpecific);
    CategoryBuilder.AddCustomRow(LOCTEXT("BufferPresentingFiltering", "Buffer Presenting"))
        .WholeRowContent()
        [
            SNew(SButton)
            .HAlign(HAlign_Center)
        .Text(LOCTEXT("PresentText", "Present Metaball Generator"))
        .OnClicked(this, &FMetaballCoordinatorDetailCustomization::OnMetaballGeneratorPresent)
        ];

    CategoryBuilder.AddCustomRow(LOCTEXT("BufferPresentingFiltering", "Buffer Presenting"))
        .WholeRowContent()
        [
            SNew(SButton)
            .HAlign(HAlign_Center)
            .Text(LOCTEXT("ShutdownText", "Shutdown"))
            .OnClicked(this, &FMetaballCoordinatorDetailCustomization::OnShutdown)
        ];
}

FReply FMetaballCoordinatorDetailCustomization::OnMetaballGeneratorPresent()
{
    if (ObjectsBeingCustomized.Num() == 1)
    {
        AMetaballCoordinator* MetaballCoordinator = Cast<AMetaballCoordinator>(ObjectsBeingCustomized[0].Get());

        if (MetaballCoordinator)
        {
            UBufferPresentingEditorSubsystem* BufferPresentingSubsystem = GEditor->GetEditorSubsystem<UBufferPresentingEditorSubsystem>();
            if (BufferPresentingSubsystem)
            {
                BufferPresentingSubsystem->Present(MetaballCoordinator->GetMetaballGeneratorComponent()->GetBuffer());
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