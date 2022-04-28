// Fill out your copyright notice in the Description page of Project Settings.

#include "BufferRampPass.h"

#include "RenderingThread.h"
#include "RHICommandList.h"

#include "BufferRampShader.h"
#include "BufferPostProcessorShaderUtility.h"

void FBufferRampPass::Process(UTextureRenderTarget2D* InputRT, UTextureRenderTarget2D* OutputRT, const FBufferRampPassSettings& PassSettings, UTexture2DDynamic* RampTexture)
{
    check(InputRT);
    check(OutputRT);
    check(InputRT->SizeX == OutputRT->SizeX && InputRT->SizeY == OutputRT->SizeY);
    check(InputRT->RenderTargetFormat == OutputRT->RenderTargetFormat);

    if (!RampTexture)
    {
        RampTexture = CreateRampTexture();
        FBufferPostProcessorShaderUtility::RenderColorRampToTexture(PassSettings.RampCurve, RampTexture);
    }

    ENQUEUE_RENDER_COMMAND(BufferRampPass)(
        [InputRT, OutputRT, RampTexture](FRHICommandListImmediate& RHICmdList)
        {
            FBufferRampShaderParameter ShaderParameter;
            ShaderParameter.InputTextureResource = InputRT->GetRenderTargetResource();
            ShaderParameter.RampTextureResource = RampTexture->GetResource();

            FIntPoint Size(OutputRT->SizeX, OutputRT->SizeY);

            FBufferRampShader::Render(RHICmdList, OutputRT->GetRenderTargetResource(), Size, ShaderParameter);
        }
    );
}

UTexture2DDynamic* FBufferRampPass::CreateRampTexture()
{
    FTexture2DDynamicCreateInfo CreateInfo(PF_B8G8R8A8, false, true, TF_Bilinear, AM_Clamp);
    UTexture2DDynamic* Result = UTexture2DDynamic::Create(256, 1, CreateInfo);

    FBufferPostProcessorShaderUtility::WaitForGPU();

    return Result;
}
