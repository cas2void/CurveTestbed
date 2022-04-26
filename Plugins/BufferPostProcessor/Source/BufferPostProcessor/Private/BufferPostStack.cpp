// Fill out your copyright notice in the Description page of Project Settings.

#include "BufferPostStack.h"

#include "BufferPostProcessorShaderUtility.h"

bool FBufferPostStackLayer::IsValid() const
{
    return bEnabled;
}

bool FBufferPostStackSettings::HasEffect() const
{
    bool Result = false;

    if (bEnabled && Layers.Num() > 0)
    {
        for (const auto& Layer : Layers)
        {
            if (Layer.IsValid())
            {
                Result = true;
                break;
            }
        }
    }

    return Result;
}

int32 FBufferPostStackSettings::GetNumValidLayers() const
{
    int32 Result = 0;

    for (int32 Index = 0; Index < Layers.Num(); Index++)
    {
        if (Layers[Index].IsValid())
        {
            Result++;
        }
    }

    return Result;
}

void FBufferPostStack::Process(UTextureRenderTarget2D* InOutRT, UTextureRenderTarget2D* IntermediateRT, const FBufferPostStackSettings& Settings)
{
    // Enabled, and has at least one valid layer.
    if (Settings.HasEffect())
    {
        UTextureRenderTarget2D* PingPong[2] = { InOutRT, IntermediateRT };

        int32 NumValidLayers = Settings.GetNumValidLayers();
        int32 ValidLayersNum = 0;
        
        for (int32 Index = 0; Index < Settings.Layers.Num(); Index++)
        {
            // Finished processing all valid layers
            if (ValidLayersNum >= NumValidLayers)
            {
                break;
            }

            if (Settings.Layers[Index].IsValid())
            {
                int32 SourceIndex = ValidLayersNum % 2;
                int32 DestIndex = 1 - SourceIndex;

                Settings.Layers[Index].Pass->Process(PingPong[SourceIndex], PingPong[DestIndex]);
                
                ValidLayersNum++;
            }
        }

        // Make sure the end of stack is InOutRT.
        if (NumValidLayers % 2 == 1)
        {
            FBufferPostProcessorShaderUtility::CopyRenderTarget(IntermediateRT, InOutRT);
        }
    }
}

TArray<UClass*> UBufferPostPass::GetAllPassClasses()
{
    TArray<UClass*> Result;

    for (TObjectIterator<UClass> It; It; ++It)
    {
        UClass* CurrentClass = *It;
        if (!CurrentClass->HasAnyClassFlags(CLASS_Abstract) && CurrentClass->IsChildOf(UBufferPostPass::StaticClass()))
        {
            Result.Add(CurrentClass);
        }
    }

    return Result;
}

FString UBufferPostPass::GetDisplayName(UClass* Class)
{
    FString Result = FName(NAME_None).ToString();

    if (Class)
    {
        FName MetaKey(TEXT("DisplayName"));
        if (Class->HasMetaData(MetaKey))
        {
            Result = Class->GetMetaData(MetaKey);
        }
    }

    return Result;
}