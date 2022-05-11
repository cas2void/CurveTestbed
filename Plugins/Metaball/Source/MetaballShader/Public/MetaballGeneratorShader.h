// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RHICommandList.h"
#include "Engine/TextureRenderTarget2D.h"

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
};
