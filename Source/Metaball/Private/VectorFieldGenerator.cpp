// Fill out your copyright notice in the Description page of Project Settings.


#include "VectorFieldGenerator.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetRenderingLibrary.h"

#include "CanvasUserWidget.h"
#include "VectorFieldShader.h"

// Sets default values
AVectorFieldGenerator::AVectorFieldGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AVectorFieldGenerator::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AVectorFieldGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#if WITH_EDITOR
void AVectorFieldGenerator::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

}
#endif