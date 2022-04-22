// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Curves/CurveLinearColor.h"
#include "BufferPostPass.generated.h"

USTRUCT()
struct BUFFERPOSTPROCESSOR_API FBufferRampPassSettings
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FRuntimeCurveLinearColor RampCurve;
};

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
    static UClass* GetClassFromType(EBufferPostPassType Type);

protected:
    static TMap<EBufferPostPassType, UClass*> TypeClassMap;
};

UCLASS()
class BUFFERPOSTPROCESSOR_API UBufferRampPass : public UBufferPostPass
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FBufferRampPassSettings PassSettings;
};