// Fill out your copyright notice in the Description page of Project Settings.

#include "BufferPostStack.h"

#include "BufferPostProcessorShaderUtility.h"

bool FBufferPostStackSettings::HasEffect() const
{
    return bEnabled && Layers.Num() > 0;
}

void FBufferPostStack::Process(UTextureRenderTarget2D* InputRT, UTextureRenderTarget2D* OutputRT, const FBufferPostStackSettings& Settings)
{
    check(InputRT && OutputRT && InputRT->SizeX == OutputRT->SizeX && InputRT->SizeY == OutputRT->SizeY && InputRT->RenderTargetFormat == OutputRT->RenderTargetFormat);

    if (Settings.HasEffect())
    {
        //UE_LOG(LogTemp, Warning, TEXT("FBufferPostStack::Process"));
    }
    else
    {
        FBufferPostProcessorShaderUtility::CopyRenderTarget(InputRT, OutputRT);
    }
}