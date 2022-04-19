// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "BufferPostPass.h"
#include "BufferPostStackComponent.generated.h"

USTRUCT()
struct BUFFERPOSTPROCESSOR_API FBufferPostStackSetting
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    TArray<FName> PassNames;
};

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
    // IO
    //
public:
    void SetInput(UTextureRenderTarget2D* InputRT);

    DECLARE_MULTICAST_DELEGATE_OneParam(FResizeBufferDelegate, const FIntPoint&);
    FResizeBufferDelegate& OnResize() { return ResizeBufferDelegate; }

    UTextureRenderTarget2D* GetOutput() { return Output; }

protected:
    UPROPERTY(VisibleAnywhere, Transient, AdvancedDisplay)
    TWeakObjectPtr<UTextureRenderTarget2D> Input;

    UPROPERTY(VisibleAnywhere, Transient, AdvancedDisplay)
    UTextureRenderTarget2D* Output;

    FResizeBufferDelegate ResizeBufferDelegate;

    //
    // Post Processing Stack
    //
public:
    void Process();

protected:
    UPROPERTY(EditAnywhere)
    FBufferPostStackSetting StackSetting;
};
