// Fill out your copyright notice in the Description page of Project Settings.

#include "BufferPostPass.h"

TMap<EBufferPostPassType, UClass*> UBufferPostPass::TypeClassMap = { {EBufferPostPassType::Ramp, UBufferRampPass::StaticClass()} };

UClass* UBufferPostPass::GetClassFromType(EBufferPostPassType Type)
{
    UClass** Result = TypeClassMap.Find(Type);
    if (Result)
    {
        return *Result;
    }
    else
    {
        return nullptr;
    }
}

void UBufferRampPass::Process(UTextureRenderTarget2D* InputRT, UTextureRenderTarget2D* OutputRT)
{
    FBufferRampPass::Process(InputRT, OutputRT, PassSettings);
}
