#pragma once

#include "CoreMinimal.h"
#include "Engine/TextureRenderTarget2D.h"
#include "RHICommandList.h"

struct FVectorFieldShaderParameter
{

};

class METABALL_API FVectorFieldShader
{
public:
    static void Render(FRHICommandListImmediate& RHICmdList, UTextureRenderTarget2D* DestRT, const FIntPoint& Size, const FVectorFieldShaderParameter& ShaderParams);
};