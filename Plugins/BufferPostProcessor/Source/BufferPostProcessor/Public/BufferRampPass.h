// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Curves/CurveLinearColor.h"
#include "Engine/Texture2DDynamic.h"
#include "Engine/TextureRenderTarget2D.h"
#include "BufferRampPass.generated.h"


USTRUCT()
struct BUFFERPOSTPROCESSOR_API FBufferRampPassSettings
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FRuntimeCurveLinearColor RampCurve;

    DECLARE_MULTICAST_DELEGATE(FRampCurveModifiedDelegate);
    FRampCurveModifiedDelegate& OnRampCurveModified() { return RampCurveModifiedDelegate; }
    const FRampCurveModifiedDelegate& OnRampCurveModified() const { return RampCurveModifiedDelegate; }

protected:
    FRampCurveModifiedDelegate RampCurveModifiedDelegate;
};

/**
 * 
 */
class BUFFERPOSTPROCESSOR_API FBufferRampPass
{
public:
    static void Process(UTextureRenderTarget2D* InputRT, UTextureRenderTarget2D* OutputRT, const FBufferRampPassSettings& PassSettings, UTexture2DDynamic* RampTexture = nullptr);
    
    static UTexture2DDynamic* CreateRampTexture();
};
