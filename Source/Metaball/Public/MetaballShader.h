#pragma once

#include "CoreMinimal.h"
#include "Engine/TextureRenderTarget2D.h"
#include "RHICommandList.h"

struct FMetaballShaderParameter
{
    FVector Point0;
    FVector Point1;
    FVector Point2;
    float AspectRatio;
};

class METABALL_API FMetaballShader
{
public:
    static void Render(FRHICommandListImmediate& RHICmdList, UTextureRenderTarget2D* DestRT, const FIntPoint& Size, const FMetaballShaderParameter& ShaderParams);
};