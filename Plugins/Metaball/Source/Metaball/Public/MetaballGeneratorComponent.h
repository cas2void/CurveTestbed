// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "BufferPostQueue.h"
#include "MetaballGeneratorComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class METABALL_API UMetaballGeneratorComponent : public UActorComponent
{
    GENERATED_BODY()

public:    
    // Sets default values for this component's properties
    UMetaballGeneratorComponent();

    //
    // UActorComponent Interfaces
    //
public:
    virtual void OnRegister() override;
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    //
    // Buffer
    //
public:
    void SetSize(const FIntPoint& Size);

    DECLARE_MULTICAST_DELEGATE_OneParam(FResizeDelegate, const FIntPoint&)
    FResizeDelegate& OnResize() { return ResizeDelegate; }

    UTextureRenderTarget2D* GetOutput();
    bool IsOutputMonochrome();

protected:
    // Desired render target size, value set by SetSize() from caller.
    // As member `OutputRT` is `Transient`, it has to be checked and created in OnRegister(), 
    // so this Value need to be serialized for later use in render target creation.
    UPROPERTY()
    FIntPoint RenderTargetSize = FIntPoint(-1, -1);

    FResizeDelegate ResizeDelegate;

    UPROPERTY(VisibleInstanceOnly, Transient, AdvancedDisplay)
    UTextureRenderTarget2D* OutputRT;

    //
    // Metaball
    //
public:
    void Process();

    DECLARE_MULTICAST_DELEGATE(FProcessDelegate)
    FProcessDelegate& OnProcess() { return ProcessDelegate; }

protected:
    FProcessDelegate ProcessDelegate;

    UPROPERTY(EditAnywhere)
	FVector Point0;

	UPROPERTY(EditAnywhere)
	FVector Point1;

	UPROPERTY(EditAnywhere)
	FVector Point2;
};
