// Fill out your copyright notice in the Description page of Project Settings.

#include "BufferPresenterEditorModule.h"

#include "PropertyEditorModule.h"

#include "BufferPresentableDetailCustomization.h"
#include "BufferPresentable.h"

#define LOCTEXT_NAMESPACE "FBufferPresenterEditorModule"

void FBufferPresenterEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditorModule.RegisterCustomClassLayout(FName("IBufferPresentable"), 
		FOnGetDetailCustomizationInstance::CreateStatic(&FBufferPresentableDetailCustomization::MakeInstance));
	PropertyEditorModule.NotifyCustomizationModuleChanged();
}

void FBufferPresenterEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyEditorModule.UnregisterCustomClassLayout(FName("IBufferPresentable"));
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBufferPresenterEditorModule, BufferPresenterEditor)