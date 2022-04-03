// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "DetailLayoutBuilder.h"

/**
 * 
 */
class BUFFERPRESENTEREDITOR_API FBufferPresentableDetailCustomization : public IDetailCustomization
{
public:
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder);

	static TSharedRef<IDetailCustomization> MakeInstance();
};
