// Fill out your copyright notice in the Description page of Project Settings.

#include "BufferPostStack.h"

#include "Kismet/KismetRenderingLibrary.h"

#include "BufferPostProcessorShaderUtility.h"

void UBufferPostStack::Process(UTextureRenderTarget2D* InputRT)
{
    if (InputRT)
    {
        check(OutputRT && InputRT->SizeX == OutputRT->SizeX && InputRT->SizeY == OutputRT->SizeY && InputRT->RenderTargetFormat == OutputRT->RenderTargetFormat);

        int32 NumEnabledPasses = 0;
        for (const auto& Pass : Passes)
        {
            if (Pass.bEnabled)
            {
                NumEnabledPasses++;
            }
        }

        if (NumEnabledPasses > 0)
        {
            for (int32 Index = 0; Index < Passes.Num(); Index++)
            {
                switch (Passes[Index].Type)
                {
                case EBufferPostPassType::Ramp:
                {
                    UBufferRampPass* RampPass = Cast<UBufferRampPass>(Passes[Index].Pass);
                    if (RampPass)
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Process Ramp Pass"));
                    }
                }
                break;

                default:
                    break;
                }
            }
        }
        else
        {
            FBufferPostProcessorShaderUtility::CopyRenderTarget(InputRT, OutputRT);
        }
    }
}

bool UBufferPostStack::HasEffect()
{
    return bEnabled && Passes.Num() > 0;
}

void UBufferPostStack::SetSize(const FIntPoint& Size, ETextureRenderTargetFormat Format)
{
    OutputRT = UKismetRenderingLibrary::CreateRenderTarget2D(this, Size.X, Size.Y, Format);
    IntermediateRT = UKismetRenderingLibrary::CreateRenderTarget2D(this, Size.X, Size.Y, Format);
}
