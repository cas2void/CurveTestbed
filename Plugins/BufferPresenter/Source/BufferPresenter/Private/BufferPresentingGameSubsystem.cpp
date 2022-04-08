// Fill out your copyright notice in the Description page of Project Settings.


#include "BufferPresentingGameSubsystem.h"

#include "Materials/MaterialInterface.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/Images/SImage.h"
#include "Engine/World.h"
#include "Engine/GameViewportClient.h"
#include "Engine/Level.h"

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

    TSharedRef<SConstraintCanvas> ConstraintCanvas = SNew(SConstraintCanvas);
    ConstraintCanvas->AddSlot()
        .Offset(FMargin(0, 0, 0, 0))
        .Anchors(FAnchors(0, 0, 1, 1))
        .Alignment(FVector2D(0, 0))
        [
            SNew(SScaleBox)
            .Stretch(EStretch::ScaleToFit)
            [
                SNew(SImage)
                .Image(&OutInfrastructure.ImageBrush)
            ]
        ];

    OutInfrastructure.FullscreenWidget = ConstraintCanvas;
}

void UBufferPresentingGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    UBufferPresentingUtility::InitBufferPresentingInfrastructure(this, BufferPresentingInfrastructure);
}

void UBufferPresentingGameSubsystem::Deinitialize()
{}

void UBufferPresentingGameSubsystem::Present(UTextureRenderTarget2D* Buffer)
{
    BufferPresentingInfrastructure.ImageBrush.SetImageSize(FVector2D(Buffer->SizeX, Buffer->SizeY));
    BufferPresentingInfrastructure.BufferMID->SetTextureParameterValue(FName("Buffer"), Buffer);

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
        }
    }

    BufferPresentingInfrastructure.BufferMID->ClearParameterValues();
}

void UBufferPresentingGameSubsystem::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
    if (!InLevel && GetWorld() == InWorld)
    {
        Shutdown();
    }
}
