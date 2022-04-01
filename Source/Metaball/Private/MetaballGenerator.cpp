// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaballGenerator.h"

#include "Kismet/KismetRenderingLibrary.h"
#include "Engine/Texture2D.h"

#include "MetaballShader.h"

// Sets default values
AMetaballGenerator::AMetaballGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AMetaballGenerator::BeginPlay()
{
	Super::BeginPlay();
}

void AMetaballGenerator::PostLoad()
{
	Super::PostLoad();
}

// Called every frame
void AMetaballGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMetaballGenerator::SetRenderTargetSize(FIntPoint Size)
{
	RenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(this, Size.X, Size.Y, RTF_RGBA16f, FLinearColor::Yellow);
}

void AMetaballGenerator::Render()
{
	FMetaballShaderParameter ShaderParams;
	ShaderParams.Point0 = Point0;
	ShaderParams.Point1 = Point1;
	ShaderParams.Point2 = Point2;
	ShaderParams.AspectRatio = static_cast<float>(RenderTarget->SizeX) / static_cast<float>(RenderTarget->SizeY);

	ENQUEUE_RENDER_COMMAND(Metaball)(
		[this, ShaderParams](FRHICommandListImmediate& RHICmdList)
		{
			FMetaballShader::Render(RHICmdList, RenderTarget, FIntPoint(RenderTarget->SizeX, RenderTarget->SizeY), ShaderParams);
		}
	);

}

#if WITH_EDITOR
void AMetaballGenerator::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

}
#endif