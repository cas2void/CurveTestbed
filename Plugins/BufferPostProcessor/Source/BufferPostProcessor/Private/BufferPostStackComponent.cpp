// Fill out your copyright notice in the Description page of Project Settings.

#include "BufferPostStackComponent.h"

#include "Kismet/KismetRenderingLibrary.h"

// Sets default values for this component's properties
UBufferPostStackComponent::UBufferPostStackComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}

// Called when the game starts
void UBufferPostStackComponent::BeginPlay()
{
    Super::BeginPlay();

    // ...
    
}

// Called every frame
void UBufferPostStackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

void UBufferPostStackComponent::SetInput(UTextureRenderTarget2D* InputRT)
{
    Input = InputRT;

    if (InputRT &&
        (!Output || Output->SizeX != InputRT->SizeX || Output->SizeY != InputRT->SizeY))
    {
        Output = UKismetRenderingLibrary::CreateRenderTarget2D(this, InputRT->SizeX, InputRT->SizeY, RTF_RGBA16f, FLinearColor::White);

        // Output could be nullptr, when this function called in Constructor, where current World of `this` is nullptr.
        if (Output)
        {
            // Reprocess as the size of cooking render target has been changed.
            Process();
            ResizeBufferDelegate.Broadcast(FIntPoint(InputRT->SizeX, InputRT->SizeY));
        }
    }
}

UTextureRenderTarget2D* UBufferPostStackComponent::GetOutput()
{
    return Output;
}

void UBufferPostStackComponent::Process()
{
    UE_LOG(LogTemp, Warning, TEXT("Post Stack:: Process()"));
}

