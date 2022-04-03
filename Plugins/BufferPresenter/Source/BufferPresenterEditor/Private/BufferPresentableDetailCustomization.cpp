// Fill out your copyright notice in the Description page of Project Settings.


#include "BufferPresentableDetailCustomization.h"

#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "Internationalization/Internationalization.h"

#include "BufferPresentable.h"

#define LOCTEXT_NAMESPACE "FBufferPresentableDetailCustomization"

void FBufferPresentableDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
    TArray<TWeakObjectPtr<UObject>> ObjectsBeingCustomized;
    DetailBuilder.GetObjectsBeingCustomized(ObjectsBeingCustomized);
    for (auto& Obj : ObjectsBeingCustomized)
    {
        if (Obj.IsValid())
        {
            IBufferPresentable* BufferPresentable = Cast<IBufferPresentable>(Obj.Get());
            if (BufferPresentable)
            {
                UE_LOG(LogTemp, Warning, TEXT("Selected BufferPresentable"));
                IDetailCategoryBuilder& Cat = DetailBuilder.EditCategory(TEXT("CatName"));

                auto OnRegenerate = [BufferPresentable]
                {
                    if (BufferPresentable)
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Process BufferPresentable"));
                    }

                    return FReply::Handled();
                };

                Cat.AddCustomRow(LOCTEXT("MyButtonRowFilterString", "Search Filter Keywords"))
                    .WholeRowContent()
                    [
                        SNew(SButton)
                        .Text(LOCTEXT("RegenerateBtnText", "Regenerate List"))
                        .OnClicked_Lambda(OnRegenerate)
                    ];
            }
        }
    }
}

TSharedRef<IDetailCustomization> FBufferPresentableDetailCustomization::MakeInstance()
{
    return MakeShareable(new FBufferPresentableDetailCustomization);
}

#undef LOCTEXT_NAMESPACE