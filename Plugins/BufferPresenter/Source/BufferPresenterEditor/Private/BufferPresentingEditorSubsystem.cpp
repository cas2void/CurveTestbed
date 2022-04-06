// Fill out your copyright notice in the Description page of Project Settings.


#include "BufferPresentingEditorSubsystem.h"

#include "Widgets/SOverlay.h"
#include "LevelEditor.h"
#include "SLevelViewport.h"
#include "Widgets/SViewport.h"
#include "Engine/World.h"
#include "Engine/Level.h"

void UBufferPresentingEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    UBufferPresentingUtility::InitBufferPresentingInfrastructure(this, BufferPresentingInfrastructure);
}

void UBufferPresentingEditorSubsystem::Deinitialize()
{
}

void UBufferPresentingEditorSubsystem::Present(UTextureRenderTarget2D* Buffer)
{
    BufferPresentingInfrastructure.ImageBrush.SetImageSize(FVector2D(Buffer->SizeX, Buffer->SizeY));
    BufferPresentingInfrastructure.BufferMID->SetTextureParameterValue(FName("Buffer"), Buffer);

    UWorld* World = GetWorld();
    if (World && !World->IsGameWorld())
    {
        TSharedPtr<SOverlay> OverlayWidget = FindLevelViewportOverlay();
        if (OverlayWidget.IsValid())
        {
            OverlayWidget->AddSlot()
                .HAlign(HAlign_Center)
                .VAlign(VAlign_Center)
                [
                    BufferPresentingInfrastructure.FullscreenWidget.ToSharedRef()
                ];

            FWorldDelegates::LevelRemovedFromWorld.RemoveAll(this);
            // Widgets added to the viewport are automatically removed if the persistent level is unloaded.
            FWorldDelegates::LevelRemovedFromWorld.AddUObject(this, &UBufferPresentingEditorSubsystem::OnLevelRemovedFromWorld);
        }
    }
}

void UBufferPresentingEditorSubsystem::Shutdown()
{
    UWorld* World = GetWorld();
    if (World && !World->IsGameWorld())
    {
        TSharedPtr<SOverlay> OverlayWidget = FindLevelViewportOverlay();
        if (OverlayWidget.IsValid())
        {
            OverlayWidget->RemoveSlot(BufferPresentingInfrastructure.FullscreenWidget.ToSharedRef());

            FWorldDelegates::LevelRemovedFromWorld.RemoveAll(this);
        }
    }
}

static TSharedPtr<SOverlay> FindOverlayRecursive(TSharedRef<SWidget> Widget)
{
    if (Widget->GetTypeAsString().Equals("SOverlay"))
    {
        return StaticCastSharedRef<SOverlay>(Widget);
    }

    TSharedPtr<SOverlay> Result;
    for (int Index = 0; Index < Widget->GetChildren()->Num(); Index++)
    {
        Result = FindOverlayRecursive(Widget->GetChildren()->GetChildAt(Index));
        if (Result.IsValid())
        {
            break;
        }
    }

    return Result;
}

TSharedPtr<SOverlay> UBufferPresentingEditorSubsystem::FindLevelViewportOverlay()
{
    TSharedPtr<SOverlay> Result;

    FLevelEditorModule& LevelEditor = FModuleManager::GetModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
    TSharedPtr<SLevelViewport> LevelViewport = LevelEditor.GetFirstActiveLevelViewport();

    if (LevelViewport.IsValid() && LevelViewport->GetViewportWidget().IsValid())
    {
        Result = FindOverlayRecursive(LevelViewport->GetViewportWidget().Pin().ToSharedRef());
    }

    return Result;
}

void UBufferPresentingEditorSubsystem::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
    if (!InLevel && GetWorld() == InWorld)
    {
        Shutdown();
    }
}