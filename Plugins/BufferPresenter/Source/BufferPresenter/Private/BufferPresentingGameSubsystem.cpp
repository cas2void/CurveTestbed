// Fill out your copyright notice in the Description page of Project Settings.


#include "BufferPresentingGameSubsystem.h"

#include "Materials/MaterialInterface.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/Images/SImage.h"
#include "Engine/World.h"
#include "Engine/GameViewportClient.h"
#include "Engine/Level.h"

static FSlateBrush GBlackBrush;

void UBufferPresentingUtility::InitBufferPresentingInfrastructure(UObject* Outer, FBufferPresentingInfastructure& OutInfrastructure)
{
    static const FString kBufferMaterialPath(TEXT("Material'/BufferPresenter/UIM_BufferPresenter.UIM_BufferPresenter'"));
    UMaterialInterface* BufferMaterial = LoadObject<UMaterialInterface>(nullptr, *kBufferMaterialPath);
    if (!BufferMaterial)
    {
        UE_LOG(LogTemp, Error, TEXT("UBufferPresentingGameSubsystem::Initialize: %s NOT exsit"), *kBufferMaterialPath);
    }

    OutInfrastructure.BufferMID = UMaterialInstanceDynamic::Create(BufferMaterial, Outer);
    OutInfrastructure.ImageBrush.SetResourceObject(OutInfrastructure.BufferMID);

    GBlackBrush.TintColor = FSlateColor(FLinearColor::Black);

    TSharedRef<SConstraintCanvas> ConstraintCanvas = SNew(SConstraintCanvas);
    ConstraintCanvas->AddSlot()
        .Offset(FMargin(0, 0, 0, 0))
        .Anchors(FAnchors(0, 0, 1, 1))
        .Alignment(FVector2D(0, 0))
        [
            SNew(SOverlay)
            + SOverlay::Slot()
            [
                SNew(SImage)
                .Image(&GBlackBrush)
            ]
            + SOverlay::Slot()
            [
                SNew(SScaleBox)
                .Stretch(EStretch::ScaleToFit)
                [
                    SNew(SImage)
                    .Image(&OutInfrastructure.ImageBrush)
                ]
            ]
        ];

    OutInfrastructure.FullscreenWidget = ConstraintCanvas;
}

void UBufferPresentingGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    UBufferPresentingUtility::InitBufferPresentingInfrastructure(this, BufferPresentingInfrastructure);
    bPresenting = false;
}

void UBufferPresentingGameSubsystem::Deinitialize()
{}

void UBufferPresentingGameSubsystem::Present(UTextureRenderTarget2D* Buffer, bool bIsMonochrome)
{
    BufferPresentingInfrastructure.ImageBrush.SetImageSize(FVector2D(Buffer->SizeX, Buffer->SizeY));
    BufferPresentingInfrastructure.BufferMID->SetTextureParameterValue(FName("Buffer"), Buffer);
    BufferPresentingInfrastructure.BufferMID->SetScalarParameterValue(FName("IsMonochrome"), bIsMonochrome ? 1.0f : 0.0f);

    UWorld* World = GetWorld();
    if (World && World->IsGameWorld())
    {
        if (UGameViewportClient* ViewportClient = World->GetGameViewport())
        {
            // Prevent adding the same widget repeatedly.
            ViewportClient->RemoveViewportWidgetContent(BufferPresentingInfrastructure.FullscreenWidget.ToSharedRef());
            ViewportClient->AddViewportWidgetContent(BufferPresentingInfrastructure.FullscreenWidget.ToSharedRef());

            FWorldDelegates::LevelRemovedFromWorld.RemoveAll(this);
            // Widgets added to the viewport are automatically removed if the persistent level is unloaded.
            FWorldDelegates::LevelRemovedFromWorld.AddUObject(this, &UBufferPresentingGameSubsystem::OnLevelRemovedFromWorld);

            bPresenting = true;
        }
    }
}

void UBufferPresentingGameSubsystem::Shutdown()
{
    UWorld* World = GetWorld();
    if (World && World->IsGameWorld())
    {
        if (UGameViewportClient* ViewportClient = World->GetGameViewport())
        {
            ViewportClient->RemoveViewportWidgetContent(BufferPresentingInfrastructure.FullscreenWidget.ToSharedRef());
            FWorldDelegates::LevelRemovedFromWorld.RemoveAll(this);

            bPresenting = false;
        }
    }

    BufferPresentingInfrastructure.BufferMID->ClearParameterValues();
}

bool UBufferPresentingGameSubsystem::IsPresenting()
{
    return bPresenting;
}

void UBufferPresentingGameSubsystem::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
    if (!InLevel && GetWorld() == InWorld)
    {
        Shutdown();
    }
}
