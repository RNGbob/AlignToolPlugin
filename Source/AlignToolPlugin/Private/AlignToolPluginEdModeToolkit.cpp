// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
// Robert Gentile 2016 


#include "AlignToolPluginPrivatePCH.h"
#include "AlignToolPluginEdMode.h"
#include "AlignToolPluginEdModeToolkit.h"

#define LOCTEXT_NAMESPACE "FAlignToolPluginEdModeToolkit"


FAlignToolPluginEdModeToolkit::FAlignToolPluginEdModeToolkit()
{
}

void FAlignToolPluginEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	struct Locals
	{
		static bool IsWidgetEnabled()
		{
			return GEditor->GetSelectedActors()->Num() != 0;
		}
		

		static FReply OnButtonClick(FVector AllignAxisBools, FVector AllignRotationBool, FAlignToolPluginEdModeToolkit* toolkit)
		{
			USelection* SelectedActors = GEditor->GetSelectedActors();
			AActor* baseActor = Cast<AActor>(SelectedActors->GetSelectedObject(0));// set first selected as base of allignment
			FVector originPosBool = FVector(1, 1, 1) - AllignAxisBools;
			FString offsetValueString = toolkit->getAxisValue(originPosBool)->GetText().ToString();
			FString rotationValueString = toolkit->getRotValue(AllignRotationBool)->GetText().ToString();

			float offsetValue = FCString::Atof(*offsetValueString);
			float rotationValue = FCString::Atof(*rotationValueString);

			bool dontRot =false;
			//UE_LOG(LogTemp, Warning, TEXT("number be %f"), offsetValue);
			
			// not offsetting boy
			if (!AllignAxisBools.X && !AllignAxisBools.Y && !AllignAxisBools.Z)
			{offsetValue = 0;}

			// dont bother rotating
			if ((!AllignRotationBool.X && !AllignRotationBool.Y && !AllignRotationBool.Z))
			{
				dontRot = true;
			}

			// Let editor know that we're about to do something that we want to undo/redo
			GEditor->BeginTransaction(LOCTEXT("MoveActorsTransactionName", "AlignActors"));

			int i = 0;
			// For each selected actor
			for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
			{
				if (AActor* LevelActor = Cast<AActor>(*Iter))
				{
					LevelActor->Modify();

					FVector newPos;
						
					if (offsetValue != 0 && i != 0)
					{
						// position is alligned axis and iterated offset from base actor
						newPos = (baseActor->GetActorLocation() * AllignAxisBools) + (baseActor->GetActorLocation() * originPosBool)* i*offsetValue;
					}
					else
					{
						// position is aligned axis and original position of current actor
						newPos = (baseActor->GetActorLocation() * AllignAxisBools) + (LevelActor->GetActorLocation() * originPosBool);
					}
						
					FRotator newRot = LevelActor->GetActorRotation();
					FRotator deltaRot;
					
					if (!dontRot)
					{
						// are we setting rotation same as base actor or preset value?
						if (rotationValue == 0)
						{deltaRot = baseActor->GetActorRotation();}
						else { deltaRot = FRotator(rotationValue, rotationValue, rotationValue); }

						if (AllignRotationBool.X)
						{newRot.Pitch = deltaRot.Pitch;}
						else if (AllignRotationBool.Y)
						{newRot.Roll = deltaRot.Roll;}
						else if (AllignRotationBool.Z)
						{newRot.Yaw = deltaRot.Yaw;}
					}

					LevelActor->TeleportTo(newPos, newRot, false, true);
					++i;
				}
				
			}

			// We're done moving actors so close transaction
			GEditor->EndTransaction();

			return FReply::Handled();
		}
		

		static TSharedRef<SWidget> MakeButton(FText InLabel, const FVector AllignAxisBools, const FVector AllignRotationBool, FAlignToolPluginEdModeToolkit* toolkit)
		{
			return SNew(SButton)
				.Text(InLabel)
				.OnClicked_Static(&Locals::OnButtonClick, AllignAxisBools, AllignRotationBool, toolkit);
		}

	};


	SAssignNew(ToolkitWidget, SBorder)
		.HAlign(HAlign_Center)
		.Padding(25)
		.IsEnabled_Static(&Locals::IsWidgetEnabled)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			.Padding(50)
			[
				SNew(STextBlock)
				.AutoWrapText(true)
				.Text(LOCTEXT("HelperLabel", "AWWWW YEE BOSS CHECK THIS SHIT OUT"))
			]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					[
						Locals::MakeButton(LOCTEXT("TestButtonLabel", "Test"), FVector(0, 0, 0), FVector(0,0,0),this)
					]
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SAssignNew(testwidgetptr,SEditableTextBox)
						.MinDesiredWidth(100)
						.HintText(LOCTEXT("TestTextlabel", "type numbers here fool"))
					]
					

				]
			+ SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(HAlign_Center)
				.Padding(10)
				[
					SNew(STextBlock)
					.AutoWrapText(true)
					.Text(LOCTEXT("Instruct1Label", "Align position to first selected Actor with or without an offset"))
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				.Padding(5)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.Padding(1)
					[
						Locals::MakeButton(LOCTEXT("XAlignButtonLabel", "Align on the X Axis"), FVector(0, 1, 1), FVector(0, 0, 0),this)
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(1)
					[
						SAssignNew(mXAxisBox, SEditableTextBox)
						.MinDesiredWidth(100)
						.HintText(LOCTEXT("xOffsetTextlabel", "Seperation Offset"))
					]


				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				.Padding(5)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.Padding(1)
					[
						Locals::MakeButton(LOCTEXT("YAlignButtonLabel", "Align on the Y Axis"), FVector(1, 0, 1), FVector(0, 0, 0), this)
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(1)
					[
						SAssignNew(mYAxisBox, SEditableTextBox)
						.MinDesiredWidth(100)
						.HintText(LOCTEXT("yOffsetTextlabel", "Seperation Offset"))
					]
				]
				
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				.Padding(5)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.Padding(1)
					[
						Locals::MakeButton(LOCTEXT("ZAlignButtonLabel", "Align on the Z Axis"), FVector(1, 1, 0), FVector(0, 0, 0),this)
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(1)
					[
						SAssignNew(mZAxisBox, SEditableTextBox)
						.MinDesiredWidth(100)
						.HintText(LOCTEXT("zOffsetTextlabel", "Seperation Offset"))
					]
				]
			+ SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(HAlign_Center)
				.Padding(10)
				[
					SNew(STextBlock)
					.AutoWrapText(true)
				.Text(LOCTEXT("Instruct2Label", "Align rotation to first selected Actor or by preset rotation (360 for 0)"))
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				.Padding(5)
				[
					
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
				    .Padding(1)
					[
						Locals::MakeButton(LOCTEXT("XRotateButtonLabel", "Rotate on the X Axis"), FVector(0, 0, 0), FVector(1, 0, 0), this)
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(1)
					[
						SAssignNew(mXAxisRot, SEditableTextBox)
						.MinDesiredWidth(100)
						.HintText(LOCTEXT("XRotationTextlabel", "Set Rotation"))
					]
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				.Padding(5)
				[
					
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.Padding(1)
					[
						Locals::MakeButton(LOCTEXT("YRotateButtonLabel", "Rotate on the Y Axis"), FVector(0, 0, 0), FVector(0, 1, 0), this)
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(1)
					[
						SAssignNew(mYAxisRot, SEditableTextBox)
						.MinDesiredWidth(100)
						.HintText(LOCTEXT("YRotationTextlabel", "Set Rotation"))
					]
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				.Padding(5)
				[
					
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.Padding(1)
					[
						Locals::MakeButton(LOCTEXT("ZRotateButtonLabel", "Rotate on the Z Axis"), FVector(0, 0, 0), FVector(0, 0, 1),this)
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(1)
					[
						SAssignNew(mZAxisRot, SEditableTextBox)
						.MinDesiredWidth(100)
						.HintText(LOCTEXT("ZRotationTextlabel", "Set Rotation"))
					]
				]

		];
		
	FModeToolkit::Init(InitToolkitHost);
}

FName FAlignToolPluginEdModeToolkit::GetToolkitFName() const
{
	return FName("AlignToolPluginEdMode");
}

FText FAlignToolPluginEdModeToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("AlignToolPluginEdModeToolkit", "DisplayName", "AlignToolPluginEdMode Tool");
}

class FEdMode* FAlignToolPluginEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FAlignToolPluginEdMode::EM_AlignToolPluginEdModeId);
}

TSharedPtr<class SEditableTextBox> FAlignToolPluginEdModeToolkit::getAxisValue(FVector axisBool)
{
	if (axisBool.X)
	{
		return mXAxisBox;
	}
	else if (axisBool.Y)
	{
		return mYAxisBox;
	}
	else if (axisBool.Z)
	{
		return mZAxisBox;
	}
	
	return TSharedPtr<class SEditableTextBox>();
}

TSharedPtr<class SEditableTextBox> FAlignToolPluginEdModeToolkit::getRotValue(FVector rotBool)
{
	if (rotBool.X)
	{
		return mXAxisRot;
	}
	else if (rotBool.Y)
	{
		return mYAxisRot;
	}
	else if (rotBool.Z)
	{
		return mZAxisRot;
	}
	return TSharedPtr<class SEditableTextBox>();
}

#undef LOCTEXT_NAMESPACE
