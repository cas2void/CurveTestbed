// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaballGeneratorComponent.h"

#include "Kismet/KismetRenderingLibrary.h"
#include "Engine/Texture2DDynamic.h"

#include "MetaballGeneratorShader.h"
#include "BufferPostStack.h"

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

    SetSize(RenderTargetSize);
}

// Called when the game starts
void UMetaballGeneratorComponent::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void UMetaballGeneratorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

void UMetaballGeneratorComponent::SetSize(const FIntPoint& Size)
{
    if (Size.X > 0 && Size.Y > 0 &&
        (!DryRT || DryRT->SizeX != Size.X || DryRT->SizeY != Size.Y || !WetRT || WetRT->SizeX != Size.X || WetRT->SizeY != Size.Y))
    {
        DryRT = UKismetRenderingLibrary::CreateRenderTarget2D(this, Size.X, Size.Y, RTF_RGBA16f, FLinearColor::Yellow);
        WetRT = UKismetRenderingLibrary::CreateRenderTarget2D(this, Size.X, Size.Y, RTF_RGBA16f, FLinearColor::Blue);
        RenderTargetSize = Size;

        // RenderTarget could be nullptr, when this function gets called in Constructor, where current World of `this` is nullptr.
        if (DryRT && WetRT)
        {
            CreateColorRampTexture();
            // Reprocess as the size of cooking render target has been changed.
            Process();

            ResizeDelegate.Broadcast(Size);
        }
    }
}

UTextureRenderTarget2D* UMetaballGeneratorComponent::GetOutput()
{
    if (PostStackSettings.HasEffect())
    {
        return WetRT;
    }
    else
    {
        return DryRT;
    }
}

void UMetaballGeneratorComponent::CreateColorRampTexture()
{
    if (!ColorRampTexture)
    {
        FTexture2DDynamicCreateInfo CreateInfo(PF_B8G8R8A8, false, true, TF_Bilinear, AM_Clamp);
        ColorRampTexture = UTexture2DDynamic::Create(256, 1, CreateInfo);

        FMetaballGeneratorShader::WaitForGPU();

        UpdateColorRampTexture();
    }
}

void UMetaballGeneratorComponent::UpdateColorRampTexture()
{
    FMetaballGeneratorShader::RenderColorRampToTexture(ColorRamp, ColorRampTexture);
}

void UMetaballGeneratorComponent::Process()
{
    FMetaballGeneratorShaderParameter ShaderParam;
    ShaderParam.Point0 = Point0;
    ShaderParam.Point1 = Point1;
    ShaderParam.Point2 = Point2;
    ShaderParam.AspectRatio = static_cast<float>(DryRT->SizeX) / static_cast<float>(DryRT->SizeY);
    ShaderParam.ColorRampTexture = ColorRampTexture;

    FMetaballGeneratorShader::RenderMetaball(DryRT, FIntPoint(DryRT->SizeX, DryRT->SizeY), ShaderParam);

    if (PostStackSettings.HasEffect())
    {
        FBufferPostStack::Process(DryRT, WetRT, PostStackSettings);
    }
    
    ProcessDelegate.Broadcast();
}
