// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaballGeneratorShader.h"

#include "GlobalShader.h"
#include "RHIResources.h"
#include "Modules/ModuleManager.h"
#include "RendererInterface.h"
#include "ScreenRendering.h"
#include "CommonRenderResources.h"
#include "PipelineStateCache.h"
#include "RenderCommandFence.h"

class FMetaballGeneratorShaderPS : public FGlobalShader
{
    DECLARE_GLOBAL_SHADER(FMetaballGeneratorShaderPS);

public:
    FMetaballGeneratorShaderPS()
        : FGlobalShader()
    {}

    FMetaballGeneratorShaderPS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
        : FGlobalShader(Initializer)
    {
        Point0Param.Bind(Initializer.ParameterMap, TEXT("Point0"));
        Point1Param.Bind(Initializer.ParameterMap, TEXT("Point1"));
        Point2Param.Bind(Initializer.ParameterMap, TEXT("Point2"));
        AspectRatioParam.Bind(Initializer.ParameterMap, TEXT("AspectRatio"));
    }

    void SetParameters(FRHICommandList& RHICmdList, const FMetaballGeneratorShaderParameter& ShaderParam)
    {
        SetShaderValue(RHICmdList, RHICmdList.GetBoundPixelShader(), Point0Param, ShaderParam.Point0);
        SetShaderValue(RHICmdList, RHICmdList.GetBoundPixelShader(), Point1Param, ShaderParam.Point1);
        SetShaderValue(RHICmdList, RHICmdList.GetBoundPixelShader(), Point2Param, ShaderParam.Point2);
        SetShaderValue(RHICmdList, RHICmdList.GetBoundPixelShader(), AspectRatioParam, ShaderParam.AspectRatio);
    }

private:
    LAYOUT_FIELD(FShaderParameter, Point0Param);
    LAYOUT_FIELD(FShaderParameter, Point1Param);
    LAYOUT_FIELD(FShaderParameter, Point2Param);
    LAYOUT_FIELD(FShaderParameter, AspectRatioParam);

};

IMPLEMENT_GLOBAL_SHADER(FMetaballGeneratorShaderPS, "/MetaballShaders/MetaballGeneratorPS.usf", "MainPS", SF_Pixel);

static void RenderMetaballInternal_RenderingThread(FRHICommandListImmediate& RHICmdList, const FTexture2DRHIRef& RHITexture, const FIntPoint& Size, const FMetaballGeneratorShaderParameter& ShaderParam)
{
    IRendererModule& RendererModule = FModuleManager::GetModuleChecked<IRendererModule>("Renderer");
    FRHIRenderPassInfo RenderPassInfo(RHITexture, ERenderTargetActions::Load_Store);
    RHICmdList.BeginRenderPass(RenderPassInfo, TEXT("MetaballGenerator"));
    {
        RHICmdList.SetViewport(0.0f, 0.0f, 0.0f, static_cast<float>(Size.X), static_cast<float>(Size.Y), 1.0f);

        FGraphicsPipelineStateInitializer GraphicsPSOInit;
        RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);
        GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
        GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
        GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();

        FGlobalShaderMap* ShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);
        TShaderMapRef<FScreenVS> VertexShader(ShaderMap);
        TShaderMapRef<FMetaballGeneratorShaderPS> PixelShader(ShaderMap);
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

void FMetaballGeneratorShader::RenderMetaball(UTextureRenderTarget2D* RenderTarget, const FIntPoint& Size, const FMetaballGeneratorShaderParameter& ShaderParam)
{
    ENQUEUE_RENDER_COMMAND(MetaballGenerator)(
        [RenderTarget, Size, ShaderParam](FRHICommandListImmediate& RHICmdList)
        {
            RenderMetaballInternal_RenderingThread(RHICmdList, RenderTarget->GetRenderTargetResource()->GetRenderTargetTexture(), Size, ShaderParam);
        }
    );
}
