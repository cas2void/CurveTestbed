// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/StaticArray.h"
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
	class UMaterialInterface* UIMaterial;

	UPROPERTY(EditAnywhere)
	FIntPoint CanvasSize = FIntPoint(1920, 1080);

	UPROPERTY(EditAnywhere, meta = (UIMin = 0, UIMax = 1))
	FVector Point0;

	UPROPERTY(EditAnywhere, meta = (UIMin = 0, UIMax = 1))
	FVector Point1;

	UPROPERTY(EditAnywhere, meta = (UIMin = 0, UIMax = 1))
	FVector Point2;

	UPROPERTY(VisibleAnywhere)
	TArray<class UTextureRenderTarget2D*> VectorFieldRTs;

	UPROPERTY()
	class UMaterialInstanceDynamic* MID;

	UPROPERTY()
	class UCanvasUserWidget* Canvas;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
