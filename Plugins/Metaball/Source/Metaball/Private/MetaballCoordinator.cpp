// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaballCoordinator.h"

#include "MetaballGeneratorComponent.h"

// Sets default values
AMetaballCoordinator::AMetaballCoordinator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MetaballGenerator = CreateDefaultSubobject<UMetaballGeneratorComponent>(FName("MetaballGenerator"));
}

// Called when the game starts or when spawned
void AMetaballCoordinator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMetaballCoordinator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}