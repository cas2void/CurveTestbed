// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "BufferPresentingEditorSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class BUFFERPRESENTEREDITOR_API UBufferPresentingEditorSubsystem : public UEditorSubsystem
{
    GENERATED_BODY()

    //
    // USubsystem Interfaces
    //
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override; 
};
