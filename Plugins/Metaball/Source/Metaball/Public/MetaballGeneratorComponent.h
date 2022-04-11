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
    void ResizeBuffer(const FIntPoint& Size);
    UTextureRenderTarget2D* GetBuffer() { return RenderTarget; }

protected:
    UPROPERTY(EditAnywhere)
    FIntPoint RenderTargetSize = FIntPoint(1920, 1080);

    UPROPERTY(VisibleInstanceOnly, Transient, AdvancedDisplay)
    UTextureRenderTarget2D* RenderTarget;

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
};
