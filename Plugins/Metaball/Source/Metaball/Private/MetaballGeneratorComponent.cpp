// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaballGeneratorComponent.h"

#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"

#include "BufferPresentingGameSubsystem.h"

// Sets default values for this component's properties
UMetaballGeneratorComponent::UMetaballGeneratorComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
    bWantsInitializeComponent = true;
    // ...
}

void UMetaballGeneratorComponent::OnComponentCreated()
{
    Super::OnComponentCreated();

    UE_LOG(LogTemp, Warning, TEXT("UMetaballGeneratorComponent::OnComponentCreated"));
}

void UMetaballGeneratorComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
    Super::OnComponentDestroyed(bDestroyingHierarchy);

    UE_LOG(LogTemp, Warning, TEXT("UMetaballGeneratorComponent::OnComponentDestroyed"));
}

void UMetaballGeneratorComponent::OnRegister()
{
    Super::OnRegister();

    UE_LOG(LogTemp, Warning, TEXT("UMetaballGeneratorComponent::OnRegister"));
}

void UMetaballGeneratorComponent::OnUnregister()
{
    UE_LOG(LogTemp, Warning, TEXT("UMetaballGeneratorComponent::OnUnregister"));

    Super::OnUnregister();
}

void UMetaballGeneratorComponent::InitializeComponent()
{
    Super::InitializeComponent();

    UE_LOG(LogTemp, Warning, TEXT("UMetaballGeneratorComponent::InitializeComponent"));
}

// Called when the game starts
void UMetaballGeneratorComponent::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("UMetaballGeneratorComponent::BeginPlay"));
    // ...
    Present();
}

void UMetaballGeneratorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    UE_LOG(LogTemp, Warning, TEXT("UMetaballGeneratorComponent::EndPlay"));
}

void UMetaballGeneratorComponent::UninitializeComponent()
{
    Super::UninitializeComponent();

    UE_LOG(LogTemp, Warning, TEXT("UMetaballGeneratorComponent::UninitializeComponent"));
}

// Called every frame
void UMetaballGeneratorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

void UMetaballGeneratorComponent::Resize(const FIntPoint& Size)
{
    RenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(this, Size.X, Size.Y, RTF_RGBA16f, FLinearColor::Blue);
}

void UMetaballGeneratorComponent::Present()
{
    UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
    UBufferPresentingGameSubsystem* BufferPresentingSubsystem = GameInstance->GetSubsystem<UBufferPresentingGameSubsystem>();
    if (BufferPresentingSubsystem)
    {
        BufferPresentingSubsystem->Present(RenderTarget);
    }
}