// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/TextureRenderTarget2D.h"
#include "BufferPostPass.h"
#include "BufferPostStack.generated.h"

USTRUCT()
struct BUFFERPOSTPROCESSOR_API FBufferPostStackPass
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    bool bEnabled;

    UPROPERTY(EditAnywhere)
    EBufferPostPassType Type;

    UPROPERTY(VisibleAnywhere)
    UBufferPostPass* Pass;
};

/**
 * 
 */
UCLASS()
class BUFFERPOSTPROCESSOR_API UBufferPostStack : public UObject
{
    GENERATED_BODY()

public:
    void Process(UTextureRenderTarget2D* InputRT);

    bool HasEffect();

protected:
    UPROPERTY(EditAnywhere)
    bool bEnabled = true;

    UPROPERTY(EditAnywhere)
    TArray<FBufferPostStackPass> Passes;

    //
    // Buffers
    //
public:
    void SetSize(const FIntPoint& Size, ETextureRenderTargetFormat Format = RTF_RGBA16f);

    UTextureRenderTarget2D* GetOutput() { return OutputRT; }

protected:
    UPROPERTY(VisibleInstanceOnly, Transient, AdvancedDisplay)
    UTextureRenderTarget2D* IntermediateRT;

    UPROPERTY(VisibleInstanceOnly, Transient, AdvancedDisplay)
    UTextureRenderTarget2D* OutputRT;
};
