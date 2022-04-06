// Fill out your copyright notice in the Description page of Project Settings.


#include "BufferPresentingGameSubsystem.h"

#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/Images/SImage.h"
#include "Styling/SlateBrush.h"
#include "Engine/World.h"
#include "Engine/GameViewportClient.h"
#include "Engine/Level.h"

UBufferPresentingGameSubsystem::UBufferPresentingGameSubsystem()
    : UGameInstanceSubsystem()
{
    ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("Material'/BufferPresenter/UIM_BufferPresenter.UIM_BufferPresenter'"));
    if (MaterialAsset.Succeeded())
    {
        BufferMID = UMaterialInstanceDynamic::Create(MaterialAsset.Object, this, FName("BufferMID"));
    }
}

void UBufferPresentingGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    TSharedRef<SConstraintCanvas> ConstraintCanvas = SNew(SConstraintCanvas);

    ImageBrush = MakeShared<FSlateBrush>();
    ImageBrush->SetResourceObject(BufferMID);

    ConstraintCanvas->AddSlot()
        .Offset(FMargin(0, 0, 0, 0))
        .Anchors(FAnchors(0, 0, 1, 1))
        .Alignment(FVector2D(0, 0))
        [
            SNew(SScaleBox)
            .Stretch(EStretch::ScaleToFit)
            [
                SNew(SImage)
                .Image(ImageBrush.Get())
            ]
        ];

    FullScreenWidget = ConstraintCanvas;
}

void UBufferPresentingGameSubsystem::Deinitialize()
{
}

void UBufferPresentingGameSubsystem::Present(UTextureRenderTarget2D* Buffer)
{
    ImageBrush->SetImageSize(FVector2D(Buffer->SizeX, Buffer->SizeY));
    BufferMID->SetTextureParameterValue(FName("Buffer"), Buffer);

    UWorld* World = GetWorld();
    if (World && World->IsGameWorld())
    {
        if (UGameViewportClient* ViewportClient = World->GetGameViewport())
        {
            // Use 10 as the zorder when adding to the viewport to avoid 
            // displaying below any built-in controls, like the virtual joysticks on mobile builds.
            ViewportClient->AddViewportWidgetContent(FullScreenWidget.ToSharedRef(), 10);

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
            ViewportClient->RemoveViewportWidgetContent(FullScreenWidget.ToSharedRef());
            FWorldDelegates::LevelRemovedFromWorld.RemoveAll(this);
        }
    }
}

void UBufferPresentingGameSubsystem::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
    if (!InLevel && GetWorld() == InWorld)
    {
        Shutdown();
    }
}
