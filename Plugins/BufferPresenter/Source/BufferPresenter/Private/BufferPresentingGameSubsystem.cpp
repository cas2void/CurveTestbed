// Fill out your copyright notice in the Description page of Project Settings.


#include "BufferPresentingGameSubsystem.h"

void UBufferPresentingGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
}

void UBufferPresentingGameSubsystem::Deinitialize()
{
}

void UBufferPresentingGameSubsystem::Present(UTextureRenderTarget2D* Buffer)
{
    UE_LOG(LogTemp, Warning, TEXT("UBufferPresentingGameSubsystem::Present"));
}

void UBufferPresentingGameSubsystem::Shutdown()
{
    UE_LOG(LogTemp, Warning, TEXT("UBufferPresentingGameSubsystem::Shutdown"));
}