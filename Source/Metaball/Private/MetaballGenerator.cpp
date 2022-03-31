// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaballGenerator.h"

#include "Kismet/KismetRenderingLibrary.h"

#include "MetaballShader.h"

// Sets default values
AMetaballGenerator::AMetaballGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMetaballGenerator::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMetaballGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

void AMetaballGenerator::SetRenderTargetSize(FIntPoint Size)
{
	RenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(this, Size.X, Size.Y, RTF_RGBA16f, FLinearColor::Yellow);
}

#if WITH_EDITOR
void AMetaballGenerator::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

}
#endif