// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MetaballActor.generated.h"

UCLASS()
class METABALL_API AMetaballActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMetaballActor();

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
	UPROPERTY(EditAnywhere)
	class UMaterialInterface* Material;

	UPROPERTY(VisibleAnywhere)
	class UTextureRenderTarget2D* VectorFieldRTA;

	UPROPERTY(VisibleAnywhere)
	class UMaterialInstanceDynamic* MID;

	UPROPERTY(VisibleAnywhere)
	class UCanvasUserWidget* Canvas;
};
