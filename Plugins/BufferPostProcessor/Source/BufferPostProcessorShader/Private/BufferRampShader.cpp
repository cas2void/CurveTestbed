// Fill out your copyright notice in the Description page of Project Settings.

#include "BufferRampShader.h"

#include "GlobalShader.h"
#include "RHIResources.h"
#include "RHICommandList.h"
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
        InputTextureParam.Bind(Initializer.ParameterMap, TEXT("InputTexture"));
        InputTextureSamplerParam.Bind(Initializer.ParameterMap, TEXT("InputTextureSampler"));
        RampTextureParam.Bind(Initializer.ParameterMap, TEXT("RampTexture"));
        RampTextureSamplerParam.Bind(Initializer.ParameterMap, TEXT("RampTextureSampler"));
    }

    void SetParameters(FRHICommandList& RHICmdList, const FBufferRampShaderParameter& ShaderParam)
    {
        if (ShaderParam.InputTextureResource)
        {
            SetTextureParameter(RHICmdList, RHICmdList.GetBoundPixelShader(), InputTextureParam, InputTextureSamplerParam,
                TStaticSamplerState<SF_Bilinear>::GetRHI(), ShaderParam.InputTextureResource->GetTexture2DRHI());
        }

        if (ShaderParam.RampTextureResource)
        {
            SetTextureParameter(RHICmdList, RHICmdList.GetBoundPixelShader(), RampTextureParam, RampTextureSamplerParam,
                TStaticSamplerState<SF_Bilinear>::GetRHI(), ShaderParam.RampTextureResource->GetTexture2DRHI());
        }
    }

private:
    LAYOUT_FIELD(FShaderResourceParameter, InputTextureParam);
    LAYOUT_FIELD(FShaderResourceParameter, InputTextureSamplerParam);
    LAYOUT_FIELD(FShaderResourceParameter, RampTextureParam);
    LAYOUT_FIELD(FShaderResourceParameter, RampTextureSamplerParam);
};

IMPLEMENT_GLOBAL_SHADER(FBufferRampShaderPS, "/BufferPostProcessorShaders/BufferRampShaderPS.usf", "MainPS", SF_Pixel);

void FBufferRampShader::Render(FRHICommandListImmediate& RHICmdList, FTextureRenderTargetResource* OutputTextureResource, const FIntPoint& Size, const FBufferRampShaderParameter& ShaderParam)
{
    check(OutputTextureResource);

    IRendererModule& RendererModule = FModuleManager::GetModuleChecked<IRendererModule>("Renderer");
    FRHIRenderPassInfo RenderPassInfo(OutputTextureResource->GetRenderTargetTexture(), ERenderTargetActions::Load_Store);
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