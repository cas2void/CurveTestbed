// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Curves/CurveLinearColor.h"
#include "Engine/Texture2DDynamic.h"
#include "Engine/TextureRenderTarget2D.h"

/**
 * 
 */
class BUFFERPOSTPROCESSORSHADER_API FBufferPostProcessorShaderUtility
{
public:
    static void CopyRenderTarget(UTextureRenderTarget2D* SourceRT, UTextureRenderTarget2D* DestRT);

    static void WaitForGPU();

    static void RenderColorRampToTexture(const FRuntimeCurveLinearColor& ColorRamp, UTexture2DDynamic* ColorRampTexture);
};
