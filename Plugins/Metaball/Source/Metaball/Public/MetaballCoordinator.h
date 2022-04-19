// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TextureRenderTarget2D.h"
#include "MetaballGeneratorComponent.h"
#include "BufferPostStackComponent.h"
#include "MetaballCoordinator.generated.h"

UCLASS()
class METABALL_API AMetaballCoordinator : public AActor
{
    GENERATED_BODY()
    
public:    
    // Sets default values for this actor's properties
    AMetaballCoordinator();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    //
    // Components
    //
public:
    UMetaballGeneratorComponent* GetMetaballGeneratorComponent() { return MetaballGenerator; }
    UBufferPostStackComponent* GetBufferPostStackComponent() { return PostStack; }
    FIntPoint GetRenderTargetSize() { return RenderTargetSize; }

protected:
    UPROPERTY(VisibleAnywhere)
    UMetaballGeneratorComponent* MetaballGenerator;

    UPROPERTY(VisibleAnywhere)
    UBufferPostStackComponent* PostStack;

    UPROPERTY(EditAnywhere)
    FIntPoint RenderTargetSize = FIntPoint(1920, 1080);

    void OnMetaballGeneratorResize(const FIntPoint& Size);
    void OnPostStackResize(const FIntPoint& Size);

    //
    // Presenting
    //
protected:
    class UBufferPresentingGameSubsystem* GetBufferPresentingSubsystem();
};
