// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "BufferPresentable.h"

#include "BufferPresentingGameSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class BUFFERPRESENTER_API UBufferPresentingGameSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
    
    //
    // USubsystem Interfaces
    //
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    //
    //
    //
public:
    void PresentBuffer(IBufferPresentable* BufferPresentable);
};
