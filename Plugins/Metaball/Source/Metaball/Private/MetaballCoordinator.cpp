// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaballCoordinator.h"

#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

#include "BufferPostStackComponent.h"
#include "BufferPresentingGameSubsystem.h"

// Sets default values
AMetaballCoordinator::AMetaballCoordinator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MetaballGenerator = CreateDefaultSubobject<UMetaballGeneratorComponent>(FName("MetaballGenerator"));
	PostStack = CreateDefaultSubobject<UBufferPostStackComponent>(FName("PostStack"));

	MetaballGenerator->OnResize().RemoveAll(this);
	MetaballGenerator->OnResize().AddUObject(this, &AMetaballCoordinator::OnMetaballGeneratorResize);

	PostStack->OnResize().RemoveAll(this);
	PostStack->OnResize().AddUObject(this, &AMetaballCoordinator::OnPostStackResize);

	MetaballGenerator->SetSize(RenderTargetSize);
}

// Called when the game starts or when spawned
void AMetaballCoordinator::BeginPlay()
{
	Super::BeginPlay();

	UBufferPresentingGameSubsystem* PresentingSubsystem = GetBufferPresentingSubsystem();
	if (PresentingSubsystem)
	{
		PresentingSubsystem->Present(PostStack->GetOutput());
	}
}

// Called every frame
void AMetaballCoordinator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMetaballCoordinator::OnMetaballGeneratorResize(const FIntPoint& Size)
{
	PostStack->SetInput(MetaballGenerator->GetBuffer());
}

void AMetaballCoordinator::OnPostStackResize(const FIntPoint& Size)
{
	// Present
	UBufferPresentingGameSubsystem* PresentingSubsystem = GetBufferPresentingSubsystem();
	if (PresentingSubsystem && PresentingSubsystem->IsPresenting())
	{
		PresentingSubsystem->Present(PostStack->GetOutput());
	}
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
