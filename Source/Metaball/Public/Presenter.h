// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Presenter.generated.h"

UCLASS()
class METABALL_API APresenter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APresenter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool ShouldTickIfViewportsOnly() const override { return true; }

	virtual void Destroyed() override;

	//
	//
	//
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class AMetaballGenerator* MetaballGenerator;

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* UIMaterial;

	UPROPERTY(EditAnywhere)
	FIntPoint CanvasSize = FIntPoint(1920, 1080);

	UPROPERTY(Transient)
	class UMaterialInstanceDynamic* MID;

	UPROPERTY(Transient)
	class UCanvasUserWidget* Canvas;

#if WITH_EDITOR
	UFUNCTION(CallInEditor)
	void Show();

	UFUNCTION(CallInEditor)
	void Hide();

	TSharedPtr<class SOverlay> FindLevelViewportOverlay();

	//UPROPERTY(Transient)
	TSharedPtr<class STextBlock> TextBlock;
#endif
};
