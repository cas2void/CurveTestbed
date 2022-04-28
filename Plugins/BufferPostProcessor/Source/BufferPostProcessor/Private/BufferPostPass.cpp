// Fill out your copyright notice in the Description page of Project Settings.

#include "BufferPostPass.h"

#include "BufferPostProcessorShaderUtility.h"

void UBufferRampPass::PostInitProperties()
{
    Super::PostInitProperties();

    RampTexture = FBufferRampPass::CreateRampTexture();
    FBufferPostProcessorShaderUtility::RenderColorRampToTexture(PassSettings.RampCurve, RampTexture);

    PassSettings.OnRampCurveModified().RemoveAll(this);
    PassSettings.OnRampCurveModified().AddUObject(this, &UBufferRampPass::OnRampCurveModified);
}

void UBufferRampPass::Process(UTextureRenderTarget2D* InputRT, UTextureRenderTarget2D* OutputRT)
{
    FBufferRampPass::Process(InputRT, OutputRT, PassSettings, RampTexture);
}

void UBufferRampPass::OnRampCurveModified()
{
    FBufferPostProcessorShaderUtility::RenderColorRampToTexture(PassSettings.RampCurve, RampTexture);
}
