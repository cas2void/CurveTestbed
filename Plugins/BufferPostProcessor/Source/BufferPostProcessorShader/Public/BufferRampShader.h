// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RHICommandList.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/Texture2DDynamic.h"

struct FBufferRampShaderParameter
{
    FVector MyVector;
    TWeakObjectPtr<UTexture2DDynamic> MyTexture;
};

/**
 * 
 */
class BUFFERPOSTPROCESSORSHADER_API FBufferRampShader
{
public:
    static void Render(UTextureRenderTarget2D* RenderTarget, const FIntPoint& Size, const FBufferRampShaderParameter& ShaderParam);
};
