// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaballGeneratorComponent.h"

#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"

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

    Resize(RenderTargetSize);
}

// Called when the game starts
void UMetaballGeneratorComponent::BeginPlay()
{
    Super::BeginPlay();

    // ...
    Present();
}

// Called every frame
void UMetaballGeneratorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

void UMetaballGeneratorComponent::Resize(const FIntPoint& Size)
{
    if (!RenderTarget ||
        RenderTarget->SizeX != Size.X ||
        RenderTarget->SizeY != Size.Y)
    {
        RenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(this, Size.X, Size.Y, RTF_RGBA16f, FLinearColor::Blue);
        RenderTargetSize = Size;
    }
}

void UMetaballGeneratorComponent::Present()
{
    UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
    UBufferPresentingGameSubsystem* BufferPresentingSubsystem = GameInstance->GetSubsystem<UBufferPresentingGameSubsystem>();
    BufferPresentingSubsystem->Present(RenderTarget);
}