// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaballActor.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetRenderingLibrary.h"

#include "CanvasUserWidget.h"
#include "VectorFieldShader.h"

// Sets default values
AMetaballActor::AMetaballActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VectorFieldRTs.Reserve(2);
}

// Called when the game starts or when spawned
void AMetaballActor::BeginPlay()
{
	Super::BeginPlay();

	check(UIMaterial);
	MID = UMaterialInstanceDynamic::Create(UIMaterial, this);

	for (int32 Index = 0; Index < 2; Index++)
	{
		UTextureRenderTarget2D* RT = UKismetRenderingLibrary::CreateRenderTarget2D(this, CanvasSize.X, CanvasSize.Y, RTF_RGBA8, FLinearColor::Yellow);
		VectorFieldRTs.Add(RT);
	}
	MID->SetTextureParameterValue(FName("RT"), VectorFieldRTs[0]);

	Canvas = CreateWidget<UCanvasUserWidget>(GetWorld());
	Canvas->SetImageSize(CanvasSize);
	Canvas->SetImageMaterial(MID);
	Canvas->AddToViewport();
}

// Called every frame
void AMetaballActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVectorFieldShaderParameter ShaderParams;

	ENQUEUE_RENDER_COMMAND(VectorField)(
		[this, ShaderParams](FRHICommandListImmediate& RHICmdList)
		{
			FVectorFieldShader::Render(RHICmdList, VectorFieldRTs[0], CanvasSize, ShaderParams);
		}
	);
}

