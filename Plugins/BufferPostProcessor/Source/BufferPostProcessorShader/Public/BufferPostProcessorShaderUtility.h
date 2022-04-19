// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TextureRenderTarget2D.h"

/**
 * 
 */
class BUFFERPOSTPROCESSORSHADER_API FBufferPostProcessorShaderUtility
{
public:
    static void CopyRenderTarget(UTextureRenderTarget2D* SourceRT, UTextureRenderTarget2D* DestRT);
};
