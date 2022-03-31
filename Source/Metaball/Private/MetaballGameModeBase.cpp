// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaballGameModeBase.h"

#include "Engine/World.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "MetaballGenerator.h"
#include "CanvasUserWidget.h"

void AMetaballGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    if (MetaballGeneratorClass)
    {
        MetaballGenerator = Cast<AMetaballGenerator>(GetWorld()->SpawnActor(MetaballGeneratorClass, NULL, NULL));
    }
    else
    {
        MetaballGenerator = GetWorld()->SpawnActor<AMetaballGenerator>();
    }
    MetaballGenerator->SetRenderTargetSize(CanvasSize);

    check(UIMaterial);
    MID = UMaterialInstanceDynamic::Create(UIMaterial, this);
    MID->SetTextureParameterValue(FName("RT"), MetaballGenerator->GetRenderTarget());

    Canvas = CreateWidget<UCanvasUserWidget>(GetWorld());
    Canvas->SetImageSize(CanvasSize);
    Canvas->SetImageMaterial(MID);
    Canvas->AddToViewport();
}