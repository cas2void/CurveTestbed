// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Curves/CurveLinearColor.h"
#include "BufferRampPass.generated.h"


USTRUCT()
struct BUFFERPOSTPROCESSOR_API FBufferRampPassSettings
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FRuntimeCurveLinearColor RampCurve;
};

/**
 * 
 */
class BUFFERPOSTPROCESSOR_API FBufferRampPass
{
public:
    static void Process(UTextureRenderTarget2D* InputRT, UTextureRenderTarget2D* OutputRT, const FBufferRampPassSettings& PassSettings);
};
