// Fill out your copyright notice in the Description page of Project Settings.


#include "CanvasUserWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ScaleBox.h"
#include "Components/ScaleBoxSlot.h"
#include "Components/Image.h"

static const FName kRTImageName = FName("RTImage");

TSharedRef<SWidget> UCanvasUserWidget::RebuildWidget()
{
    UCanvasPanel* RootWidget = WidgetTree->ConstructWidget<UCanvasPanel>();
    UScaleBox* ScaleBox = WidgetTree->ConstructWidget<UScaleBox>();
    ScaleBox->SetStretch(EStretch::ScaleToFit);  // Default
    UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(RootWidget->AddChild(ScaleBox));
    if (CanvasPanelSlot)
    {
        CanvasPanelSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
        CanvasPanelSlot->SetOffsets(FMargin(0.0f));
    }

    UImage* ImageWidget = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), kRTImageName);
    ImageWidget->SetBrushSize(ImageSize);
    if (ImageMaterial.IsValid())
    {
        ImageWidget->SetBrushFromMaterial(ImageMaterial.Get());
    }
    UScaleBoxSlot* ScaleBoxSlot = Cast<UScaleBoxSlot>(ScaleBox->AddChild(ImageWidget));
    if (ScaleBoxSlot)
    {
        ScaleBoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);  // Default
        ScaleBoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);      // Default
    }

    WidgetTree->RootWidget = RootWidget;

    return Super::RebuildWidget();
}

void UCanvasUserWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UCanvasUserWidget::SetImageMaterial(class UMaterialInterface* Material)
{
    ImageMaterial = Material;
}

void UCanvasUserWidget::SetImageSize(const FIntPoint& Size)
{
    ImageSize = FVector2D(Size);
}