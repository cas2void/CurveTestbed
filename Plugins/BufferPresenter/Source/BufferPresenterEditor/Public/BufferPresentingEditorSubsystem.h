// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "BufferPresentingGameSubsystem.h"
#include "BufferPresentingEditorSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class BUFFERPRESENTEREDITOR_API UBufferPresentingEditorSubsystem : public UEditorSubsystem
{
    GENERATED_BODY()

    //
    // USubsystem Interfaces
    //
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    
    //
    // Buffer Presenting
    //
public:
    void Present(UTextureRenderTarget2D* Buffer);
    void Shutdown();

protected:
    TSharedPtr<class SOverlay> FindLevelViewportOverlay();
    
    bool IsRunningGame(class UBufferPresentingGameSubsystem** OutGameSubsystem = nullptr);
    void PresentInEditingMode(UTextureRenderTarget2D* Buffer);
    void ShutdownInEditingMode();

    void OnStartGameInstance(class UGameInstance* GameInstance);
    void OnWorldCleanup(class UWorld* World, bool bSessionEnded, bool bCleanupResources);

    UPROPERTY(Transient)
    FBufferPresentingInfastructure BufferPresentingInfrastructure;
};
