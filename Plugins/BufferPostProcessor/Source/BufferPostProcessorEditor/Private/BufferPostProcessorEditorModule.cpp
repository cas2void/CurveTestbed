// Fill out your copyright notice in the Description page of Project Settings.

#include "BufferPostProcessorEditorModule.h"

#include "PropertyEditorModule.h"

#include "BufferPostStack.h"
#include "BufferPostStackTypeCustomization.h"

#define LOCTEXT_NAMESPACE "FBufferPostProcessorEditorModule"

void FBufferPostProcessorEditorModule::StartupModule()
{
    // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
    FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    PropertyEditorModule.RegisterCustomPropertyTypeLayout(FBufferPostStackSettings::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FBufferPostStackSettingsTypeCustomization::MakeInstance));
    PropertyEditorModule.RegisterCustomPropertyTypeLayout(FBufferPostStackLayer::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FBufferPostStackLayerTypeCustomization::MakeInstance));
    //PropertyEditorModule.NotifyCustomizationModuleChanged();
}

void FBufferPostProcessorEditorModule::ShutdownModule()
{
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
    if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
    {
        FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
        PropertyEditorModule.UnregisterCustomPropertyTypeLayout(FBufferPostStackSettings::StaticStruct()->GetFName());
        PropertyEditorModule.UnregisterCustomPropertyTypeLayout(FBufferPostStackLayer::StaticStruct()->GetFName());
        //PropertyEditorModule.NotifyCustomizationModuleChanged();
    }
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FBufferPostProcessorEditorModule, BufferPostProcessorEditor)