// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "Input/Reply.h"

/**
 * 
 */
class METABALLEDITOR_API FMetaballGeneratorComponentDetailCustomization : public IDetailCustomization
{
public:
    static TSharedRef<IDetailCustomization> MakeInstance();

    //
    // IDetailCustomization Interfaces
    //
    virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

protected:
    void OnPostStackSettingsChanged(class UMetaballGeneratorComponent* MetaballGenerator);
};
