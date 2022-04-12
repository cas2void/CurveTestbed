// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RHICommandList.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Curves/CurveLinearColor.h"
#include "Engine/Texture2DDynamic.h"

struct FMetaballGeneratorShaderParameter
{
    FVector Point0;
    FVector Point1;
    FVector Point2;
    float AspectRatio;
};

/**
 * 
 */
class METABALLSHADER_API FMetaballGeneratorShader
{
public:
    static void RenderMetaball(UTextureRenderTarget2D* RenderTarget, const FIntPoint& Size, const FMetaballGeneratorShaderParameter& ShaderParam);

    static void WaitForGPU();

    static void RenderColorRampToTexture(const FRuntimeCurveLinearColor& ColorRamp, UTexture2DDynamic* ColorRampTexture);
};
