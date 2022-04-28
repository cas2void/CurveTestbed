// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BufferPostQueue.h"
#include "BufferRampPass.h"
#include "BufferPostPass.generated.h"

/**
 *
 */
UCLASS(editinlinenew, meta = (DisplayName = "Ramp"))
class BUFFERPOSTPROCESSOR_API UBufferRampPass : public UBufferPostPass
{
    GENERATED_BODY()

    //
    // UObject Interfaces
    //
public:
    virtual void PostInitProperties() override;
    virtual void PostLoad() override;

    //
    // UBufferPostPass Interfaces
    //
public:
    UPROPERTY(EditAnywhere)
    FBufferRampPassSettings PassSettings;

    virtual void Process(UTextureRenderTarget2D* InputRT, UTextureRenderTarget2D* OutputRT) override;

    //
    // Ramp Texture
    //
protected:
    UPROPERTY(VisibleInstanceOnly, Transient, AdvancedDisplay)
    UTexture2DDynamic* RampTexture;

    void OnRampCurveModified();
};