// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BufferPresentable.h"
#include "MetaballGenerator.generated.h"

UCLASS()
class METABALL_API AMetaballGenerator : public AActor, public IBufferPresentable
{
    GENERATED_BODY()
    
public:    
    // Sets default values for this actor's properties
    AMetaballGenerator();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    //
    //
    //
public:
    virtual UTextureRenderTarget2D* GetBuffer() override;
};
