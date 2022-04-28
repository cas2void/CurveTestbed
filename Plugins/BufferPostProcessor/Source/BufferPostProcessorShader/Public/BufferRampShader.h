// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TextureResource.h"
#include "RHICommandList.h"

struct FBufferRampShaderParameter
{
    FTextureRenderTargetResource* InputTextureResource;
    FTextureResource* RampTextureResource;
};

/**
 * 
 */
class BUFFERPOSTPROCESSORSHADER_API FBufferRampShader
{
public:
    static void Render(FRHICommandListImmediate& RHICmdList, FTextureRenderTargetResource* OutputTextureResource, const FIntPoint& Size, const FBufferRampShaderParameter& ShaderParam);
};
