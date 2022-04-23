// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/TextureRenderTarget2D.h"

#include "BufferRampPass.h"

#include "BufferPostPass.generated.h"

/**
 *
 */
UENUM()
enum class EBufferPostPassType : uint8
{
    None,
    Ramp
};

/**
 * 
 */
UCLASS()
class BUFFERPOSTPROCESSOR_API UBufferPostPass : public UObject
{
    GENERATED_BODY()

public:
    virtual void Process(UTextureRenderTarget2D* InputRT, UTextureRenderTarget2D* OutputRT) {};
    
    //
    // Class < > Type Conversion
    //
public:
    static UClass* GetClassFromType(EBufferPostPassType Type);

protected:
    static TMap<EBufferPostPassType, UClass*> TypeClassMap;
};

/**
 *
 */
UCLASS()
class BUFFERPOSTPROCESSOR_API UBufferRampPass : public UBufferPostPass
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FBufferRampPassSettings PassSettings;

    virtual void Process(UTextureRenderTarget2D* InputRT, UTextureRenderTarget2D* OutputRT) override;
};