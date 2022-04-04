// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
    //
    //
protected:
    UPROPERTY(VisibleAnywhere)
    class UMetaballGeneratorComponent* MetaballGenerator;
};
