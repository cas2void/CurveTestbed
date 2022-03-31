// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MetaballGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class METABALL_API AMetaballGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	//
	// AActor Interfaces
	//
protected:
	virtual void BeginPlay() override;

	//
	//
	//
protected:
	UPROPERTY(EditAnywhere)
	class TSubclassOf<class AMetaballGenerator> MetaballGeneratorClass;

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* UIMaterial;

	UPROPERTY(EditAnywhere)
	FIntPoint CanvasSize = FIntPoint(1920, 1080);

	UPROPERTY(Transient)
	class AMetaballGenerator* MetaballGenerator;

	UPROPERTY(Transient)
	class UMaterialInstanceDynamic* MID;

	UPROPERTY(Transient)
	class UCanvasUserWidget* Canvas;
};
