// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TextureRenderTarget2D.h"
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
    //
    //
public:
    void Resize(const FIntPoint& Size);
    void Present();
    UTextureRenderTarget2D* GetBuffer() { return RenderTarget; }

protected:
    UPROPERTY(EditAnywhere)
    FIntPoint RenderTargetSize = FIntPoint(1920, 1080);

    UPROPERTY(VisibleAnywhere, Transient)
    UTextureRenderTarget2D* RenderTarget;
};
