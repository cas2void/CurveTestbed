// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaballGeneratorComponent.h"

#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Texture2DDynamic.h"

#include "MetaballGeneratorShader.h"
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

        FMetaballGeneratorShader::WaitForGPU();

        UpdateColorRampTexture();
    }
}

void UMetaballGeneratorComponent::UpdateColorRampTexture()
{
    FMetaballGeneratorShader::RenderColorRampToTexture(ColorRamp, ColorRampTexture);
}

void UMetaballGeneratorComponent::Render()
{
    FMetaballGeneratorShaderParameter ShaderParam;
    ShaderParam.Point0 = Point0;
    ShaderParam.Point1 = Point1;
    ShaderParam.Point2 = Point2;
    ShaderParam.AspectRatio = static_cast<float>(RenderTarget->SizeX) / static_cast<float>(RenderTarget->SizeY);

    FMetaballGeneratorShader::RenderMetaball(RenderTarget, FIntPoint(RenderTarget->SizeX, RenderTarget->SizeY), ShaderParam);
}
