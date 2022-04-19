// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Curves/CurveLinearColor.h"
#include "BufferPostPass.generated.h"

/**
 * 
 */
class BUFFERPOSTPROCESSOR_API IBufferPostPass
{
public:
};

USTRUCT()
struct FBufferRampPassSetting
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FRuntimeCurveLinearColor RampCurve;
};