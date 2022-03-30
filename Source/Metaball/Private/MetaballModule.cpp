#include "MetaballModule.h"

#include "Misc/Paths.h"
#include "ShaderCore.h"

#define LOCTEXT_NAMESPACE "FMetaballModule"

DEFINE_LOG_CATEGORY(LogMetaball)

void FMetaballModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	UE_LOG(LogMetaball, Log, TEXT("StartupModule"));
	FString ShaderDirectory = FPaths::Combine(FPaths::ProjectDir(), TEXT("Shaders"), TEXT("Metaball"), TEXT("Private"));
	AddShaderSourceDirectoryMapping("/MetaballShaders", ShaderDirectory);
}

void FMetaballModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMetaballModule, Metaball)