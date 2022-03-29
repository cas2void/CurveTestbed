// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaballActor.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "CanvasUserWidget.h"

// Sets default values
AMetaballActor::AMetaballActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMetaballActor::BeginPlay()
{
	Super::BeginPlay();

	check(Material);
	VectorFieldRTA = UKismetRenderingLibrary::CreateRenderTarget2D(this, 1280, 720, RTF_RGBA8, FLinearColor::Yellow);
	MID = UMaterialInstanceDynamic::Create(Material, this);
	MID->SetTextureParameterValue(FName("RT"), VectorFieldRTA);

	Canvas = CreateWidget<UCanvasUserWidget>(GetWorld());
	Canvas->SetImageMaterial(MID);
	Canvas->AddToViewport();
}

// Called every frame
void AMetaballActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

