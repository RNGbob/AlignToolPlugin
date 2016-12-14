// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

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
			float offsetValue = FCString::Atof(*offsetValueString);
			//FString testText = toolkit->testContent()->GetText().ToString();
			//float testfloat = FCString::Atof(*testText);
			UE_LOG(LogTemp, Warning, TEXT("number be %f"), offsetValue);
			
			
			// Let editor know that we're about to do something that we want to undo/redo
			GEditor->BeginTransaction(LOCTEXT("MoveActorsTransactionName", "AlignActors"));

			// For each selected actor
			for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
			{
				if (AActor* LevelActor = Cast<AActor>(*Iter))
				{
					LevelActor->Modify();

					//if (LevelActor != baseActor)
					//{
						// returns the non transformed point bool
					
						FVector newPos = (baseActor->GetActorLocation() * AllignAxisBools) + (LevelActor->GetActorLocation() * originPosBool);
						FRotator newRot = LevelActor->GetActorRotation();
					
						if (AllignRotationBool.X)
						{newRot.Pitch = baseActor->GetActorRotation().Pitch;}
						else if (AllignRotationBool.Y)
						{newRot.Roll = baseActor->GetActorRotation().Roll;}
						else if (AllignRotationBool.Z)
						{newRot.Yaw = baseActor->GetActorRotation().Yaw;}


						LevelActor->TeleportTo(newPos, newRot, false, true);
					//}
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
						.MinDesiredWidth(120)
						.HintText(LOCTEXT("TestTextlabel", "type numbers here fool"))
					]
					

				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					[
						Locals::MakeButton(LOCTEXT("XAlignButtonLabel", "Align on the X Axis"), FVector(0, 1, 1), FVector(0, 0, 0),this)
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						SAssignNew(mXAxisBox, SEditableTextBox)
						.MinDesiredWidth(120)
						.HintText(LOCTEXT("xOffsetTextlabel", "seperation Offset"))
					]


				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					[
						Locals::MakeButton(LOCTEXT("YAlignButtonLabel", "Align on the Y Axis"), FVector(1, 0, 1), FVector(0, 0, 0), this)
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						SAssignNew(mYAxisBox, SEditableTextBox)
						.MinDesiredWidth(120)
						.HintText(LOCTEXT("yOffsetTextlabel", "seperation Offset"))
					]
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					[
						Locals::MakeButton(LOCTEXT("ZAlignButtonLabel", "Align on the Z Axis"), FVector(1, 1, 0), FVector(0, 0, 0),this)
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						SAssignNew(mZAxisBox, SEditableTextBox)
						.MinDesiredWidth(120)
						.HintText(LOCTEXT("zOffsetTextlabel", "seperation Offset"))
					]
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					Locals::MakeButton(LOCTEXT("XRotateButtonLabel", "Rotate on the X Axis"), FVector(0, 0, 0), FVector(1, 0, 0), this)
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					Locals::MakeButton(LOCTEXT("YRotateButtonLabel", "Rotate on the Y Axis"), FVector(0, 0, 0), FVector(0, 1, 0), this)
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					Locals::MakeButton(LOCTEXT("ZRotateButtonLabel", "Rotate on the Z Axis"), FVector(0, 0, 0), FVector(0, 0, 1),this)
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

#undef LOCTEXT_NAMESPACE
