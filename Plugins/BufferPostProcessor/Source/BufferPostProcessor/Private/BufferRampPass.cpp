// Fill out your copyright notice in the Description page of Project Settings.

#include "BufferRampPass.h"

#include "BufferRampShader.h"

void FBufferRampPass::Process(UTextureRenderTarget2D* InputRT, UTextureRenderTarget2D* OutputRT, const FBufferRampPassSettings& PassSettings)
{
    check(InputRT);
    check(OutputRT);
    check(InputRT->SizeX == OutputRT->SizeX && InputRT->SizeY == OutputRT->SizeY);
    check(InputRT->RenderTargetFormat == OutputRT->RenderTargetFormat);

    FBufferRampShaderParameter ShaderParameter;
    
    FIntPoint Size(OutputRT->SizeX, OutputRT->SizeY);

    FBufferRampShader::Render(OutputRT, Size, ShaderParameter);
}
