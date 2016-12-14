// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Editor/UnrealEd/Public/Toolkits/BaseToolkit.h"


class FAlignToolPluginEdModeToolkit : public FModeToolkit
{
public:

	FAlignToolPluginEdModeToolkit();
	
	/** FModeToolkit interface */
	virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost) override;

	/** IToolkit interface */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual class FEdMode* GetEditorMode() const override;
	virtual TSharedPtr<class SWidget> GetInlineContent() const override { return ToolkitWidget; }
	TSharedPtr<class SEditableTextBox> testContent() const { return testwidgetptr; }
	TSharedPtr<class SEditableTextBox> getAxisValue(FVector axisBool);
	TSharedPtr<class SEditableTextBox> getRotValue(FVector rotBool);
private:

	TSharedPtr<SWidget> ToolkitWidget;
	TSharedPtr<SEditableTextBox> testwidgetptr;
	TSharedPtr<SEditableTextBox> mXAxisBox;
	TSharedPtr<SEditableTextBox> mYAxisBox;
	TSharedPtr<SEditableTextBox> mZAxisBox;
	TSharedPtr<SEditableTextBox> mXAxisRot;
	TSharedPtr<SEditableTextBox> mYAxisRot;
	TSharedPtr<SEditableTextBox> mZAxisRot;

};
