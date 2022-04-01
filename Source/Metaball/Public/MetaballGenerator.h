// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Curves/CurveLinearColor.h"
#include "MetaballGenerator.generated.h"

UCLASS()
class METABALL_API AMetaballGenerator: public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMetaballGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostLoad() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//
	//
	//
public:
	void SetRenderTargetSize(FIntPoint Size);
	UTextureRenderTarget2D* GetRenderTarget() const { return RenderTarget; }
	void Render();

protected:
	UPROPERTY(EditAnywhere, meta = (UIMin = 0, UIMax = 1))
	FVector Point0;

	UPROPERTY(EditAnywhere, meta = (UIMin = 0, UIMax = 1))
	FVector Point1;

	UPROPERTY(EditAnywhere, meta = (UIMin = 0, UIMax = 1))
	FVector Point2;

	UPROPERTY(VisibleAnywhere)
	FRuntimeCurveLinearColor ColorRamp;

	UPROPERTY(VisibleAnywhere, Transient)
	class UTexture2D* ColorRampTexture;

	UPROPERTY(VisibleAnywhere)
	UTextureRenderTarget2D* RenderTarget;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
