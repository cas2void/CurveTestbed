// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CanvasUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class METABALL_API UCanvasUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
	//
	// UUserWidget Interfaces
	//
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;

	//
	//
	//
public:
	void SetImageMaterial(class UMaterialInterface* Material);
	void SetImageSize(const FIntPoint& Size);

protected:
	UPROPERTY()
	TWeakObjectPtr<class UMaterialInterface> ImageMaterial;

	UPROPERTY()
	FVector2D ImageSize;
};
