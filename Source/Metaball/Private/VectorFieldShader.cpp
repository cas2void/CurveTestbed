#include "VectorFieldShader.h"

#include "GlobalShader.h"
#include "Modules/ModuleManager.h"
#include "RendererInterface.h"
#include "CommonRenderResources.h"
#include "PipelineStateCache.h"
#include "ScreenRendering.h"

class FVectorFieldShaderPS : public FGlobalShader
{
    DECLARE_GLOBAL_SHADER(FVectorFieldShaderPS);

public:
    FVectorFieldShaderPS()
        : FGlobalShader()
    {

    }

    FVectorFieldShaderPS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
        : FGlobalShader(Initializer)
    {
        Point0Param.Bind(Initializer.ParameterMap, TEXT("Point0"));
        Point1Param.Bind(Initializer.ParameterMap, TEXT("Point1"));
        Point2Param.Bind(Initializer.ParameterMap, TEXT("Point2"));
        AspectRatioParam.Bind(Initializer.ParameterMap, TEXT("AspectRatio"));
    }

    void SetParameters(FRHICommandList& RHICmdList, const FVectorFieldShaderParameter& ShaderParams)
    {
        SetShaderValue(RHICmdList, RHICmdList.GetBoundPixelShader(), Point0Param, ShaderParams.Point0);
        SetShaderValue(RHICmdList, RHICmdList.GetBoundPixelShader(), Point1Param, ShaderParams.Point1);
        SetShaderValue(RHICmdList, RHICmdList.GetBoundPixelShader(), Point2Param, ShaderParams.Point2);
        SetShaderValue(RHICmdList, RHICmdList.GetBoundPixelShader(), AspectRatioParam, ShaderParams.AspectRatio);
    }

private:
    LAYOUT_FIELD(FShaderParameter, Point0Param);
    LAYOUT_FIELD(FShaderParameter, Point1Param);
    LAYOUT_FIELD(FShaderParameter, Point2Param);
    LAYOUT_FIELD(FShaderParameter, AspectRatioParam);
};

IMPLEMENT_GLOBAL_SHADER(FVectorFieldShaderPS, "/MetaballShaders/VectorFieldPS.usf", "MainPS", SF_Pixel);

void FVectorFieldShader::Render(FRHICommandListImmediate& RHICmdList, UTextureRenderTarget2D* DestRT, const FIntPoint& Size, const FVectorFieldShaderParameter& ShaderParams)
{
    if (DestRT && DestRT->GetRenderTargetResource())
    {
        const FTexture2DRHIRef& RHITexture = DestRT->GetRenderTargetResource()->GetRenderTargetTexture();

        IRendererModule* RendererModule = &FModuleManager::GetModuleChecked<IRendererModule>("Renderer");
        FRHIRenderPassInfo RPInfo(RHITexture, ERenderTargetActions::Load_Store);
        RHICmdList.BeginRenderPass(RPInfo, TEXT("VectorField"));
        {
            RHICmdList.SetViewport(0, 0, 0, Size.X, Size.Y, 1);

            FGraphicsPipelineStateInitializer GraphicsPSOInit;
            RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);
            GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
            GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
            GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();

            FGlobalShaderMap* ShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);
            TShaderMapRef<FScreenVS> VertexShader(ShaderMap);
            TShaderMapRef<FVectorFieldShaderPS> PixelShader(ShaderMap);
            PixelShader->SetParameters(RHICmdList, ShaderParams);

            GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GFilterVertexDeclaration.VertexDeclarationRHI;
            GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
            GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();
            GraphicsPSOInit.PrimitiveType = PT_TriangleList;

            SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit);

            RendererModule->DrawRectangle(
                RHICmdList,
                0, 0,             // Dest X, Y
                Size.X, Size.Y,   // Dest Width, Height
                0, 0,             // Source U, V
                1, 1,             // Source USize, VSize
                Size,             // Target buffer size
                FIntPoint(1, 1),  // Source texture size
                VertexShader);
        }
        RHICmdList.EndRenderPass();
    }
}