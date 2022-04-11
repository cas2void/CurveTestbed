// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaballGeneratorComponent.h"

#include "Engine/TextureRenderTarget2D.h"
#include "Engine/Texture2DDynamic.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "TextureResource.h"
#include "RHIResources.h"
#include "RHICommandList.h"
#include "RenderCommandFence.h"

#include "BufferPresentingGameSubsystem.h"

// Sets default values for this component's properties
UMetaballGeneratorComponent::UMetaballGeneratorComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
    // ...
}

void UMetaballGeneratorComponent::OnRegister()
{
    Super::OnRegister();

    ResizeBuffer(RenderTargetSize);
    CreateColorRampTexture();
}

// Called when the game starts
void UMetaballGeneratorComponent::BeginPlay()
{
    Super::BeginPlay();

    // Present
    UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
    UBufferPresentingGameSubsystem* BufferPresentingSubsystem = GameInstance->GetSubsystem<UBufferPresentingGameSubsystem>();
    BufferPresentingSubsystem->Present(RenderTarget);
}

// Called every frame
void UMetaballGeneratorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

void UMetaballGeneratorComponent::ResizeBuffer(const FIntPoint& Size)
{
    if (!RenderTarget ||
        RenderTarget->SizeX != Size.X ||
        RenderTarget->SizeY != Size.Y)
    {
        RenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(this, Size.X, Size.Y, RTF_RGBA16f, FLinearColor::Blue);
        RenderTargetSize = Size;
    }
}

void UMetaballGeneratorComponent::CreateColorRampTexture()
{
    if (!ColorRampTexture)
    {
        FTexture2DDynamicCreateInfo CreateInfo(PF_B8G8R8A8, false, true, TF_Bilinear, AM_Clamp);
        ColorRampTexture = UTexture2DDynamic::Create(256, 1, CreateInfo);

        FRenderCommandFence Fence;
        Fence.BeginFence();
        Fence.Wait();
        UpdateColorRampTexture();
    }
}

void UMetaballGeneratorComponent::UpdateColorRampTexture()
{
    if (ColorRampTexture && ColorRampTexture->GetResource())
    {
        FRHITexture2D* RHITexture2D = ColorRampTexture->GetResource()->GetTexture2DRHI();
        if (RHITexture2D)
        {
            ENQUEUE_RENDER_COMMAND(UpdateColorRampTexture)(
                [RHITexture2D, this](FRHICommandListImmediate& RHICmdList)
                {
                    uint32 DestStride;
                    FColor* Buffer = static_cast<FColor*>(RHILockTexture2D(RHITexture2D, 0, RLM_WriteOnly, DestStride, false));
                    if (Buffer)
                    {
                        uint32 TextureWidth = RHITexture2D->GetSizeX();
                        for (uint32 Index = 0; Index < TextureWidth; Index++)
                        {
                            float SampleTime = static_cast<float>(Index) / static_cast<float>(TextureWidth);
                            FColor SampledColor = ColorRamp.GetLinearColorValue(SampleTime).ToFColor(false);
                            Buffer[Index] = SampledColor;
                        }
                    }
                    RHIUnlockTexture2D(RHITexture2D, 0, false);
                });
        }
    }
}
