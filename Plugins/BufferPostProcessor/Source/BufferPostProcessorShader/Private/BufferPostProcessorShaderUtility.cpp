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
