// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/TextureRenderTarget2D.h"
#include "BufferPostStack.generated.h"

/**
 * 
 */
UCLASS()
class BUFFERPOSTPROCESSOR_API UBufferPostStack : public UObject
{
    GENERATED_BODY()
    
public:
    void Process(UTextureRenderTarget2D* Input, UTextureRenderTarget2D* Output);

    bool HasEffect();
};
