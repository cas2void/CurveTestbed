// Fill out your copyright notice in the Description page of Project Settings.

#include "BufferPostStackTypeCustomization.h"

#include "DetailLayoutBuilder.h"
#include "PropertyHandle.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"

#include "BufferPostStack.h"

#define LOCTEXT_NAMESPACE "BufferPostStackTypeCustomization"

TSharedRef<IPropertyTypeCustomization> FBufferPostStackTypeCustomization::MakeInstance()
{
    return MakeShareable(new FBufferPostStackTypeCustomization);
}

void FBufferPostStackTypeCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    UE_LOG(LogTemp, Warning, TEXT("FBufferPostStackTypeCustomization::CustomizeHeader"));
}

void FBufferPostStackTypeCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    UE_LOG(LogTemp, Warning, TEXT("FBufferPostStackTypeCustomization::CustomizeChildren"));
}

#undef LOCTEXT_NAMESPACE