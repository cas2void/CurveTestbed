// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/TextureRenderTarget2D.h"
#include "BufferPostPass.h"
#include "BufferPostStack.generated.h"

/**
 *
 */
USTRUCT()
struct BUFFERPOSTPROCESSOR_API FBufferPostStackLayer
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    bool bEnabled = true;

    UPROPERTY(EditAnywhere)
    EBufferPostPassType Type;

    UPROPERTY(EditAnywhere, Instanced)
    UBufferPostPass* Pass;

    bool IsValid() const;
};

/**
 *
 */
USTRUCT()
struct BUFFERPOSTPROCESSOR_API FBufferPostStackSettings
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    bool bEnabled = true;

    UPROPERTY(EditAnywhere)
    TArray<FBufferPostStackLayer> Layers;

    bool HasEffect() const;

    int32 GetNumValidLayers() const;
};

/**
 *
 */
class BUFFERPOSTPROCESSOR_API FBufferPostStack
{
public:
    static void Process(UTextureRenderTarget2D* InOutRT, UTextureRenderTarget2D* IntermediateRT, const FBufferPostStackSettings& Settings);
};