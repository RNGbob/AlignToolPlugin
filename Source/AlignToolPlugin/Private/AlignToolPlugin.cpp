// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "AlignToolPluginPrivatePCH.h"
#include "AlignToolPluginEdMode.h"

#define LOCTEXT_NAMESPACE "FAlignToolPluginModule"

void FAlignToolPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FAlignToolPluginEdMode>(FAlignToolPluginEdMode::EM_AlignToolPluginEdModeId, LOCTEXT("AlignToolPluginEdModeName", "AlignToolPlugin"), FSlateIcon(), true);
}

void FAlignToolPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FAlignToolPluginEdMode::EM_AlignToolPluginEdModeId);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAlignToolPluginModule, AlignToolPlugin)