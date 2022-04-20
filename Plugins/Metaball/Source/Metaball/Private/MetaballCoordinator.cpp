// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaballCoordinator.h"

#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

#include "BufferPresentingGameSubsystem.h"

// Sets default values
AMetaballCoordinator::AMetaballCoordinator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MetaballGenerator = CreateDefaultSubobject<UMetaballGeneratorComponent>(FName("MetaballGenerator"));

	MetaballGenerator->OnResize().RemoveAll(this);
	MetaballGenerator->OnResize().AddUObject(this, &AMetaballCoordinator::OnMetaballGeneratorResize);
	MetaballGenerator->OnProcess().RemoveAll(this);
	MetaballGenerator->OnProcess().AddUObject(this, &AMetaballCoordinator::OnMetaballProcess);

	MetaballGenerator->SetSize(RenderTargetSize);
}

// Called when the game starts or when spawned
void AMetaballCoordinator::BeginPlay()
{
	Super::BeginPlay();

	UBufferPresentingGameSubsystem* PresentingSubsystem = GetBufferPresentingSubsystem();
	if (PresentingSubsystem)
	{
		PresentingSubsystem->Present(MetaballGenerator->GetOutput());
	}
}

// Called every frame
void AMetaballCoordinator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMetaballCoordinator::OnMetaballGeneratorResize(const FIntPoint& Size)
{
	// Present
	UBufferPresentingGameSubsystem* PresentingSubsystem = GetBufferPresentingSubsystem();
	if (PresentingSubsystem && PresentingSubsystem->IsPresenting())
	{
		PresentingSubsystem->Present(MetaballGenerator->GetOutput());
	}
}

void AMetaballCoordinator::OnMetaballProcess()
{
}

UBufferPresentingGameSubsystem* AMetaballCoordinator::GetBufferPresentingSubsystem()
{
	UBufferPresentingGameSubsystem* Result = nullptr;

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (GameInstance)
	{
		Result = GameInstance->GetSubsystem<UBufferPresentingGameSubsystem>();
	}

	return Result;
}
