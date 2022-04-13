// Fill out your copyright notice in the Description page of Project Settings.


#include "BufferPresentingEditorSubsystem.h"

#include "LevelEditor.h"
#include "SLevelViewport.h"
#include "Widgets/SViewport.h"
#include "Layout/Children.h"
#include "Widgets/SOverlay.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

#include "BufferPresentingGameSubsystem.h"

void UBufferPresentingEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    UBufferPresentingUtility::InitBufferPresentingInfrastructure(this, BufferPresentingInfrastructure);

    FWorldDelegates::OnStartGameInstance.RemoveAll(this);
    FWorldDelegates::OnStartGameInstance.AddUObject(this, &UBufferPresentingEditorSubsystem::OnStartGameInstance);
}

void UBufferPresentingEditorSubsystem::Deinitialize()
{
    FWorldDelegates::OnStartGameInstance.RemoveAll(this);
}

void UBufferPresentingEditorSubsystem::Present(UTextureRenderTarget2D* Buffer)
{
    UBufferPresentingGameSubsystem* GameSubsystem;
    if (IsRunningGame(&GameSubsystem))
    {
        GameSubsystem->Present(Buffer);
    }
    else
    {
        PresentInEditingMode(Buffer);
    }
}

void UBufferPresentingEditorSubsystem::Shutdown()
{
    UBufferPresentingGameSubsystem* GameSubsystem;
    if (IsRunningGame(&GameSubsystem))
    {
        GameSubsystem->Shutdown();
    }
    else
    {
        ShutdownInEditingMode();
    }
}

TSharedPtr<SOverlay> UBufferPresentingEditorSubsystem::FindLevelViewportOverlay()
{
    TSharedPtr<SOverlay> Result;

    FLevelEditorModule& LevelEditor = FModuleManager::GetModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
    TSharedPtr<SLevelViewport> LevelViewport = LevelEditor.GetFirstActiveLevelViewport();

    if (LevelViewport.IsValid() && LevelViewport->GetViewportWidget().IsValid())
    {
        TArray<FString> TypeStringPath = { "SOverlay", "SGameLayerManager", "SDPIScaler", "SVerticalBox", "SOverlay" };
        TSharedPtr<SWidget> FoundWidget = FindChildWidgetOfType(LevelViewport->GetViewportWidget().Pin().ToSharedRef(), TypeStringPath);
        Result = StaticCastSharedPtr<SOverlay>(FoundWidget);
    }

    return Result;
}

TSharedPtr<SWidget> UBufferPresentingEditorSubsystem::FindChildWidgetOfType(TSharedRef<SWidget> Widget, const TArray<FString>& TypeStringPath)
{
    TSharedPtr<SWidget> Result;

    if (TypeStringPath.Num() > 0)
    {
        Result = Widget;

        for (const auto& TypeString : TypeStringPath)
        {
            bool bNotFound = true;

            FChildren* Children = Result->GetChildren();
            if (Children)
            {
                for (int Index = 0; Index < Children->Num(); Index++)
                {
                    TSharedRef<SWidget> Child = Children->GetChildAt(Index);
                    if (Child->GetTypeAsString().Equals(TypeString, ESearchCase::IgnoreCase))
                    {
                        Result = Child;
                        bNotFound = false;
                        break;
                    }
                }
            }

            // Quit if any type string in the path is not found
            if (bNotFound)
            {
                Result = nullptr;
                break;
            }
        }
    }

    return Result;
}

bool UBufferPresentingEditorSubsystem::IsRunningGame(UBufferPresentingGameSubsystem** OutGameSubsystem)
{
    bool Result = false;
    for (const auto& Context : GEngine->GetWorldContexts())
    {
        if (Context.WorldType == EWorldType::Game || Context.WorldType == EWorldType::PIE)
        {
            if (OutGameSubsystem)
            {
                UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(Context.World());
                if (GameInstance)
                {
                    UBufferPresentingGameSubsystem* GameSubsystem = GameInstance->GetSubsystem<UBufferPresentingGameSubsystem>();
                    if (GameSubsystem)
                    {
                        Result = true;
                        *OutGameSubsystem = GameSubsystem;
                    }
                }
            }
            else
            {
                Result = true;
            }
            break;
        }
    }
    return Result;
}

void UBufferPresentingEditorSubsystem::PresentInEditingMode(UTextureRenderTarget2D* Buffer)
{
    BufferPresentingInfrastructure.ImageBrush.SetImageSize(FVector2D(Buffer->SizeX, Buffer->SizeY));
    BufferPresentingInfrastructure.BufferMID->SetTextureParameterValue(FName("Buffer"), Buffer);

    TSharedPtr<SOverlay> OverlayWidget = FindLevelViewportOverlay();
    if (OverlayWidget.IsValid())
    {
        OverlayWidget->RemoveSlot(BufferPresentingInfrastructure.FullscreenWidget.ToSharedRef());
        OverlayWidget->AddSlot()
            .HAlign(HAlign_Fill)
            .VAlign(VAlign_Fill)
            [
                BufferPresentingInfrastructure.FullscreenWidget.ToSharedRef()
            ];

        FWorldDelegates::OnWorldCleanup.RemoveAll(this);
        FWorldDelegates::OnWorldCleanup.AddUObject(this, &UBufferPresentingEditorSubsystem::OnWorldCleanup);
    }
}

void UBufferPresentingEditorSubsystem::ShutdownInEditingMode()
{
    TSharedPtr<SOverlay> OverlayWidget = FindLevelViewportOverlay();
    if (OverlayWidget.IsValid())
    {
        OverlayWidget->RemoveSlot(BufferPresentingInfrastructure.FullscreenWidget.ToSharedRef());

        FWorldDelegates::OnWorldCleanup.RemoveAll(this);
    }

    BufferPresentingInfrastructure.BufferMID->ClearParameterValues();
}

void UBufferPresentingEditorSubsystem::OnStartGameInstance(UGameInstance* GameInstance)
{
    // Cleanup the viewport for game instance.
    ShutdownInEditingMode();
}

void UBufferPresentingEditorSubsystem::OnWorldCleanup(UWorld* World, bool bSessionEnded, bool bCleanupResources)
{
    // Prevent shutdown from game world cleanup, which is already done OnStartGameInstance.
    if (!World || World->WorldType == EWorldType::Editor)
    {
        ShutdownInEditingMode();
    }
}