// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/TextureRenderTarget2D.h"
#include "BufferPostQueue.generated.h"

/**
 *
 */
UCLASS(Abstract)
class BUFFERPOSTPROCESSOR_API UBufferPostPass : public UObject
{
    GENERATED_BODY()

public:
    virtual void Process(UTextureRenderTarget2D* InputRT, UTextureRenderTarget2D* OutputRT) {};

    static TArray<UClass*> GetAllPassClasses();
    static FString GetDisplayName(UClass* Class);
};

/**
 *
 */
USTRUCT()
struct BUFFERPOSTPROCESSOR_API FBufferPostQueueLayer
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    bool bEnabled = true;

    UPROPERTY(EditAnywhere, Instanced)
    UBufferPostPass* Pass;

    bool IsValid() const;
};

/**
 *
 */
USTRUCT()
struct BUFFERPOSTPROCESSOR_API FBufferPostQueueSettings
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    bool bEnabled = true;

    UPROPERTY(EditAnywhere)
    TArray<FBufferPostQueueLayer> Layers;

    bool HasEffect() const;

    int32 GetNumValidLayers() const;
};

/**
 *
 */
class BUFFERPOSTPROCESSOR_API FBufferPostQueue
{
public:
    static void Process(UTextureRenderTarget2D* InOutRT, UTextureRenderTarget2D* IntermediateRT, const FBufferPostQueueSettings& Settings);
};