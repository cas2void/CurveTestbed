// Fill out your copyright notice in the Description page of Project Settings.


#include "BufferPresentableDetailCustomization.h"

#include "BufferPresentable.h"

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
            }
        }
    }
}

TSharedRef<IDetailCustomization> FBufferPresentableDetailCustomization::MakeInstance()
{
    return MakeShareable(new FBufferPresentableDetailCustomization);
}