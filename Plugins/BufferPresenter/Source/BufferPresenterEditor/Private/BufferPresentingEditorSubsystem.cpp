// Fill out your copyright notice in the Description page of Project Settings.


#include "BufferPresentingEditorSubsystem.h"

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
        FWorldDelegates::LevelRemovedFromWorld.RemoveAll(this);
        // Widgets added to the viewport are automatically removed if the persistent level is unloaded.
        FWorldDelegates::LevelRemovedFromWorld.AddUObject(this, &UBufferPresentingEditorSubsystem::OnLevelRemovedFromWorld);
    }
}

void UBufferPresentingEditorSubsystem::Shutdown()
{
    UWorld* World = GetWorld();
    if (World && !World->IsGameWorld())
    {
        FWorldDelegates::LevelRemovedFromWorld.RemoveAll(this);
    }
}

void UBufferPresentingEditorSubsystem::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
    if (!InLevel && GetWorld() == InWorld)
    {
        Shutdown();
    }
}