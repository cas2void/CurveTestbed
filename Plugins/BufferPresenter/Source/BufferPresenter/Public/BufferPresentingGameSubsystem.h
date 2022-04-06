// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Widgets/SWidget.h"
#include "Styling/SlateBrush.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/TextureRenderTarget2D.h"
#include "BufferPresentingGameSubsystem.generated.h"

USTRUCT()
struct BUFFERPRESENTER_API FBufferPresentingInfastructure
{
    GENERATED_BODY()

    UPROPERTY(Transient)
    UMaterialInstanceDynamic* BufferMID;

    TSharedPtr<SWidget> FullscreenWidget;

    FSlateBrush ImageBrush;
};

UCLASS()
class BUFFERPRESENTER_API UBufferPresentingUtility : public UObject
{
    GENERATED_BODY()

public:
    static void InitBufferPresentingInfrastructure(UObject* Outer, FBufferPresentingInfastructure& OutInfrastructure);
};

/**
 * 
 */
UCLASS()
class BUFFERPRESENTER_API UBufferPresentingGameSubsystem : public UGameInstanceSubsystem
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
    void OnLevelRemovedFromWorld(class ULevel* InLevel, class UWorld* InWorld);

    FBufferPresentingInfastructure BufferPresentingInfrastructure;
};
