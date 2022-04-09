// Fill out your copyright notice in the Description page of Project Settings.

#include "MetaballEditorModule.h"

#include "PropertyEditorModule.h"

#include "MetaballCoordinator.h"
#include "MetaballCoordinatorDetailCustomization.h"
#include "MetaballGeneratorComponent.h"
#include "MetaballGeneratorComponentDetailCustomization.h"

#define LOCTEXT_NAMESPACE "FMetaballEditorModule"

void FMetaballEditorModule::StartupModule()
{
    // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
    FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    PropertyEditorModule.RegisterCustomClassLayout(AMetaballCoordinator::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FMetaballCoordinatorDetailCustomization::MakeInstance));
    PropertyEditorModule.RegisterCustomClassLayout(UMetaballGeneratorComponent::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FMetaballGeneratorComponentDetailCustomization::MakeInstance));
}

void FMetaballEditorModule::ShutdownModule()
{
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
    if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
    {
        FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
        PropertyEditorModule.UnregisterCustomClassLayout(AMetaballCoordinator::StaticClass()->GetFName());
        PropertyEditorModule.UnregisterCustomClassLayout(UMetaballGeneratorComponent::StaticClass()->GetFName());
        PropertyEditorModule.NotifyCustomizationModuleChanged();
    }
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FMetaballEditorModule, MetaballEditor)