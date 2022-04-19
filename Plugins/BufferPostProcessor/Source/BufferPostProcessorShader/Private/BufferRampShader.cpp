// Fill out your copyright notice in the Description page of Project Settings.

#include "BufferRampShader.h"

#include "GlobalShader.h"
#include "RHIResources.h"
#include "Modules/ModuleManager.h"
#include "RendererInterface.h"
#include "ScreenRendering.h"
#include "CommonRenderResources.h"
#include "PipelineStateCache.h"
#include "RenderCommandFence.h"

class FBufferRampShaderPS : public FGlobalShader
{
    DECLARE_GLOBAL_SHADER(FBufferRampShaderPS);

public:
    FBufferRampShaderPS()
        : FGlobalShader()
    {}

    FBufferRampShaderPS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
        : FGlobalShader(Initializer)
    {
        MyVectorParam.Bind(Initializer.ParameterMap, TEXT("MyVector"));
        MyTextureParam.Bind(Initializer.ParameterMap, TEXT("MyTexture"));
        MyTextureSamplerParam.Bind(Initializer.ParameterMap, TEXT("MyTextureSampler"));
    }

    void SetParameters(FRHICommandList& RHICmdList, const FBufferRampShaderParameter& ShaderParam)
    {
        SetShaderValue(RHICmdList, RHICmdList.GetBoundPixelShader(), MyVectorParam, ShaderParam.MyVector);
        if (ShaderParam.MyTexture.IsValid())
        {
            SetTextureParameter(RHICmdList, RHICmdList.GetBoundPixelShader(), MyTextureParam, MyTextureSamplerParam,
                TStaticSamplerState<SF_Bilinear>::GetRHI(), ShaderParam.MyTexture->GetResource()->GetTexture2DRHI());
        }
    }

private:
    LAYOUT_FIELD(FShaderParameter, MyVectorParam);
    LAYOUT_FIELD(FShaderResourceParameter, MyTextureParam);
    LAYOUT_FIELD(FShaderResourceParameter, MyTextureSamplerParam);

};

IMPLEMENT_GLOBAL_SHADER(FBufferRampShaderPS, "/BufferPostProcessorShaders/BufferRampShaderPS.usf", "MainPS", SF_Pixel);

static void Render_RenderingThread(FRHICommandListImmediate& RHICmdList, const FTexture2DRHIRef& RHITexture, const FIntPoint& Size, const FBufferRampShaderParameter& ShaderParam)
{
    IRendererModule& RendererModule = FModuleManager::GetModuleChecked<IRendererModule>("Renderer");
    FRHIRenderPassInfo RenderPassInfo(RHITexture, ERenderTargetActions::Load_Store);
    RHICmdList.BeginRenderPass(RenderPassInfo, TEXT("BufferRamp"));
    {
        RHICmdList.SetViewport(0.0f, 0.0f, 0.0f, static_cast<float>(Size.X), static_cast<float>(Size.Y), 1.0f);

        FGraphicsPipelineStateInitializer GraphicsPSOInit;
        RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);
        GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
        GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
        GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();

        FGlobalShaderMap* ShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);
        TShaderMapRef<FScreenVS> VertexShader(ShaderMap);
        TShaderMapRef<FBufferRampShaderPS> PixelShader(ShaderMap);
        PixelShader->SetParameters(RHICmdList, ShaderParam);

        GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GFilterVertexDeclaration.VertexDeclarationRHI;
        GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
        GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();
        GraphicsPSOInit.PrimitiveType = PT_TriangleList;

        SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit);

        RendererModule.DrawRectangle(
            RHICmdList,
            0.0f, 0.0f,                                              // Dest X, Y
            static_cast<float>(Size.X), static_cast<float>(Size.Y),  // Dest width, height
            0.0f, 0.0f,                                              // Source U, V
            1.0f, 1.0f,                                              // Source USize, VSize
            Size,                                                    // Target buffer size
            FIntPoint(1, 1),                                         // Source texture size
            VertexShader
        );
    }
    RHICmdList.EndRenderPass();
}

void FBufferRampShader::Render(UTextureRenderTarget2D* RenderTarget, const FIntPoint& Size, const FBufferRampShaderParameter& ShaderParam)
{
    ENQUEUE_RENDER_COMMAND(MetaballGenerator)(
        [RenderTarget, Size, ShaderParam](FRHICommandListImmediate& RHICmdList)
        {
            Render_RenderingThread(RHICmdList, RenderTarget->GetRenderTargetResource()->GetRenderTargetTexture(), Size, ShaderParam);
        }
    );
}