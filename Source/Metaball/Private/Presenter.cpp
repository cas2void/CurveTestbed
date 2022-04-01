// Fill out your copyright notice in the Description page of Project Settings.


#include "Presenter.h"

#include "Engine/World.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "MetaballGenerator.h"
#include "CanvasUserWidget.h"

// Sets default values
APresenter::APresenter()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APresenter::BeginPlay()
{
    Super::BeginPlay();

    MetaballGenerator->SetRenderTargetSize(CanvasSize);

    check(UIMaterial);
    MID = UMaterialInstanceDynamic::Create(UIMaterial, this);
    MID->SetTextureParameterValue(FName("RT"), MetaballGenerator->GetRenderTarget());

    Canvas = CreateWidget<UCanvasUserWidget>(GetWorld());
    Canvas->SetImageSize(CanvasSize);
    Canvas->SetImageMaterial(MID);
    Canvas->AddToViewport();
}

// Called every frame
void APresenter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //MetaballGenerator->Render();
    //FindOverlay();
}

void APresenter::Destroyed()
{
#if WITH_EDITOR
    Hide();
#endif
    Super::Destroyed();
}

#if WITH_EDITOR
#include "Editor/EditorEngine.h"
#include "LevelEditorViewport.h"
#include "SEditorViewport.h"
#include "Slate/SceneViewport.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/SWindow.h"
#include "Widgets/Input/SButton.h"
#include "LevelEditor.h"
#include "SLevelViewport.h"

void APresenter::Show()
{
    TSharedPtr<SOverlay> OverlayWidget = FindLevelViewportOverlay();

    if (OverlayWidget.IsValid())
    {
        if (!TextBlock.IsValid())
        {
            TextBlock = SNew(STextBlock).Text(FText::FromString("Hello"));
            OverlayWidget->AddSlot()
                .HAlign(HAlign_Center)
                .VAlign(VAlign_Center)
                [
                    TextBlock.ToSharedRef()
                ];
        }
    }
}

void APresenter::Hide()
{
    TSharedPtr<SOverlay> OverlayWidget = FindLevelViewportOverlay();

    if (OverlayWidget.IsValid())
    {
        if (TextBlock.IsValid())
        {
            OverlayWidget->RemoveSlot(TextBlock.ToSharedRef());
            TextBlock.Reset();
        }
    }
}

static TSharedPtr<SOverlay> FindOverlayRecursive(TSharedRef<SWidget> Widget)
{
    if (Widget->GetTypeAsString().Equals("SOverlay"))
    {
        return StaticCastSharedRef<SOverlay>(Widget);
    }

    TSharedPtr<SOverlay> Result;
    for (int Index = 0; Index < Widget->GetChildren()->Num(); Index++)
    {
        Result = FindOverlayRecursive(Widget->GetChildren()->GetChildAt(Index));
        if (Result.IsValid())
        {
            break;
        }
    }

    return Result;
}

TSharedPtr<class SOverlay> APresenter::FindLevelViewportOverlay()
{
    TSharedPtr<SOverlay> Result;

    FLevelEditorModule& LevelEditor = FModuleManager::GetModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
    TSharedPtr<SLevelViewport> LevelViewport = LevelEditor.GetFirstActiveLevelViewport();

    if (LevelViewport.IsValid() && LevelViewport->GetViewportWidget().IsValid())
    {
        Result = FindOverlayRecursive(LevelViewport->GetViewportWidget().Pin().ToSharedRef());
    }

    return Result;
}
#endif