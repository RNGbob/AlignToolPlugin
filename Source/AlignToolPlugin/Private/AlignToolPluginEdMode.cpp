// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "AlignToolPluginPrivatePCH.h"
#include "AlignToolPluginEdMode.h"
#include "AlignToolPluginEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"

const FEditorModeID FAlignToolPluginEdMode::EM_AlignToolPluginEdModeId = TEXT("EM_AlignToolPluginEdMode");

FAlignToolPluginEdMode::FAlignToolPluginEdMode()
{

}

FAlignToolPluginEdMode::~FAlignToolPluginEdMode()
{

}

void FAlignToolPluginEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FAlignToolPluginEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FAlignToolPluginEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FAlignToolPluginEdMode::UsesToolkits() const
{
	return true;
}




