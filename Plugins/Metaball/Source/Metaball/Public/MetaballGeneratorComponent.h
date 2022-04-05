// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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
    virtual void OnComponentCreated() override;
    virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
    virtual void OnRegister() override;
    virtual void OnUnregister() override;
    virtual void InitializeComponent() override;
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void UninitializeComponent() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    //
    //
    //
public:
    void Resize(const FIntPoint& Size);
    void Present();

protected:
    UPROPERTY(EditAnywhere)
    FIntPoint RenderTargetSize = FIntPoint(1920, 1080);

    UPROPERTY(VisibleAnywhere, Transient)
    class UTextureRenderTarget2D* RenderTarget;
};
