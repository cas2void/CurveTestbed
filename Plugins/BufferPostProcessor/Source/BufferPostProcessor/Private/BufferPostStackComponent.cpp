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

    if (Input &&
        (!Output || Output->SizeX != Input->SizeX || Output->SizeY != Input->SizeY))
    {
        Output = UKismetRenderingLibrary::CreateRenderTarget2D(this, Input->SizeX, Input->SizeY, RTF_RGBA16f, FLinearColor::White);

        ResizeBufferDelegate.Broadcast(FIntPoint(Input->SizeX, Input->SizeY));
    }
}

UTextureRenderTarget2D* UBufferPostStackComponent::GetOutput()
{
    return Output;
}

void UBufferPostStackComponent::Process()
{
}

