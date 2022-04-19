// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "BufferPostStackComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUFFERPOSTPROCESSOR_API UBufferPostStackComponent : public UActorComponent
{
    GENERATED_BODY()

public:    
    // Sets default values for this component's properties
    UBufferPostStackComponent();

    //
	// UActorComponent Interfaces
	//
public:
    // Called when the game starts
    virtual void BeginPlay() override;
 
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    //
    // Post Processing Stack
    //
public:
    void SetInput(UTextureRenderTarget2D* InputRT);
    UTextureRenderTarget2D* GetOutput();
    void Process();

    DECLARE_MULTICAST_DELEGATE_OneParam(FResizeBufferDelegate, const FIntPoint&);
    FResizeBufferDelegate& OnResize() { return ResizeBufferDelegate; }

protected:
    UPROPERTY(VisibleAnywhere)
    UTextureRenderTarget2D* Input;

    UPROPERTY(VisibleAnywhere)
    UTextureRenderTarget2D* Output;

    FResizeBufferDelegate ResizeBufferDelegate;
};
