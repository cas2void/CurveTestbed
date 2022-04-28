// Fill out your copyright notice in the Description page of Project Settings.

#include "BufferPostProcessorShaderUtility.h"

#include "RenderingThread.h"
#include "RHICommandList.h"

void FBufferPostProcessorShaderUtility::CopyRenderTarget(UTextureRenderTarget2D* SourceRT, UTextureRenderTarget2D* DestRT)
{
    if (SourceRT && DestRT)
    {
        ENQUEUE_RENDER_COMMAND(CopyRenderTarget)(
            [SourceRT, DestRT](FRHICommandListImmediate& RHICmdList)
            {
                FTextureRenderTargetResource* SourceResource = SourceRT->GetRenderTargetResource();
                FTextureRenderTargetResource* DestResource = DestRT->GetRenderTargetResource();

                if (SourceResource && DestResource)
                {
                    FResolveParams ResolveParams;
                    RHICmdList.CopyToResolveTarget(SourceResource->GetRenderTargetTexture(), DestResource->GetRenderTargetTexture(), ResolveParams);
                }
            }
        );
    }
}

void FBufferPostProcessorShaderUtility::WaitForGPU()
{
    FRenderCommandFence Fence;
    Fence.BeginFence();
    Fence.Wait();
}

void FBufferPostProcessorShaderUtility::RenderColorRampToTexture(const FRuntimeCurveLinearColor& ColorRamp, UTexture2DDynamic* ColorRampTexture)
{
    ENQUEUE_RENDER_COMMAND(UpdateColorRampTexture)(
        [ColorRamp, ColorRampTexture](FRHICommandListImmediate& RHICmdList)
        {
            FRHITexture2D* RHITexture2D = ColorRampTexture->GetResource()->GetTexture2DRHI();
            if (RHITexture2D)
            {
                uint32 DestStride;
                FColor* Buffer = static_cast<FColor*>(RHILockTexture2D(RHITexture2D, 0, RLM_WriteOnly, DestStride, false));
                if (Buffer)
                {
                    uint32 TextureWidth = RHITexture2D->GetSizeX();
                    for (uint32 Index = 0; Index < TextureWidth; Index++)
                    {
                        float SampleTime = static_cast<float>(Index) / static_cast<float>(TextureWidth);
                        FColor SampledColor = ColorRamp.GetLinearColorValue(SampleTime).ToFColor(false);
                        Buffer[Index] = SampledColor;
                    }
                }
                RHIUnlockTexture2D(RHITexture2D, 0, false);
            }
        }
    );
}