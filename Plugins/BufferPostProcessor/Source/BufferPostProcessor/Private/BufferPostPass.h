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
UCLASS(meta = (DisplayName = "Ramp"))
class BUFFERPOSTPROCESSOR_API UBufferRampPass : public UBufferPostPass
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FBufferRampPassSettings PassSettings;

    virtual void Process(UTextureRenderTarget2D* InputRT, UTextureRenderTarget2D* OutputRT) override
    {
        FBufferRampPass::Process(InputRT, OutputRT, PassSettings);
    }
};