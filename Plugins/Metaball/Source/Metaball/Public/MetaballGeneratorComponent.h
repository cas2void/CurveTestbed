// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Curves/CurveLinearColor.h"
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

    DECLARE_MULTICAST_DELEGATE_OneParam(FResizeBufferDelegate, const FIntPoint&)
    FResizeBufferDelegate& OnResize() { return ResizeBufferDelegate; }

    UTextureRenderTarget2D* GetBuffer() { return RenderTarget; }

protected:
    UPROPERTY(VisibleInstanceOnly, Transient, AdvancedDisplay)
    UTextureRenderTarget2D* RenderTarget;

    // Desired render target size, value set by SetSize() from caller.
    // As member `RenderTarget` is Transient, it has to be checked and created in OnRegister(), 
    // this Value need to be serialized for later use in render target creation.
    UPROPERTY()
    FIntPoint RenderTargetSize = FIntPoint(-1, -1);

    FResizeBufferDelegate ResizeBufferDelegate;

    //
    // Ramp
    //
public:
    void UpdateColorRampTexture();
    const FRuntimeCurveLinearColor& GetColorRamp() const { return ColorRamp; }
    
protected:
    void CreateColorRampTexture();

    UPROPERTY(EditAnywhere)
    FRuntimeCurveLinearColor ColorRamp;

    UPROPERTY(VisibleInstanceOnly, Transient, AdvancedDisplay)
    class UTexture2DDynamic* ColorRampTexture;

    //
    // Metaball
    //
public:
    void Process();

protected:
    UPROPERTY(EditAnywhere)
	FVector Point0;

	UPROPERTY(EditAnywhere)
	FVector Point1;

	UPROPERTY(EditAnywhere)
	FVector Point2;
};
