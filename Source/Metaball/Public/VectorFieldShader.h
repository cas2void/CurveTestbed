#pragma once

#include "CoreMinimal.h"
#include "Engine/TextureRenderTarget2D.h"
#include "RHICommandList.h"

struct FVectorFieldShaderParameter
{
    FVector Point0;
    FVector Point1;
    FVector Point2;
    float AspectRatio;
};

class METABALL_API FVectorFieldShader
{
public:
    static void Render(FRHICommandListImmediate& RHICmdList, UTextureRenderTarget2D* DestRT, const FIntPoint& Size, const FVectorFieldShaderParameter& ShaderParams);
};