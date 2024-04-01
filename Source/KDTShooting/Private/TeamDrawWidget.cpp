// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamDrawWidget.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"


void UTeamDrawWidget::NativeConstruct()
{
	Super::NativeConstruct();

	btn_Shuffle->OnClicked.AddDynamic(this, &UTeamDrawWidget::StartShuffle);
}

void UTeamDrawWidget::StartShuffle()
{
	teams.Add(edit_name1->GetText().ToString());
	teams.Add(edit_name2->GetText().ToString());
	teams.Add(edit_name3->GetText().ToString());
	teams.Add(edit_name4->GetText().ToString());
	teams.Add(edit_name5->GetText().ToString());
	teams.Add(edit_name6->GetText().ToString());
	teams.Add(edit_name7->GetText().ToString());

	sw_Steps->SetActiveWidgetIndex(1);
	PlayAnimationForward(anim_textShow);

	TArray<FString> result;
	result.SetNum(teams.Num());

	for (int32 i = 0; i < teams.Num(); i++)
	{
		int32 randNum = FMath::RandRange(0, teams.Num() - 1);
		result[i] = teams[randNum];

		for (int32 j = 0; j < i; j++)
		{
			if (result[j] == teams[randNum])
			{
				i--;
				break;
			}
		}
	}
	teamResults = result;

	FTimerHandle showTimer;
	GetWorld()->GetTimerManager().SetTimer(showTimer, FTimerDelegate::CreateLambda([&]()
		{
			ShowTeamResults();
		}), 3, false);
	
}

void UTeamDrawWidget::ShowTeamResults()
{
	sw_Steps->SetActiveWidgetIndex(2);

	text_name1->SetText(FText::FromString(teamResults[0]));
	text_name2->SetText(FText::FromString(teamResults[1]));
	text_name3->SetText(FText::FromString(teamResults[2]));
	text_name4->SetText(FText::FromString(teamResults[3]));
	text_name5->SetText(FText::FromString(teamResults[4]));
	text_name6->SetText(FText::FromString(teamResults[5]));
	text_name7->SetText(FText::FromString(teamResults[6]));

	PlayAnimationForward(anim_resultShow);

	FString saveText;

	for (const FString& teamName : teamResults)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s\n"), *teamName);
		FString save = teamName + "\n";
		saveText.Append(save);
	}

	FString fullPath = FPaths::ProjectContentDir() + FString("/save/TeamResult.txt");

	bool isSuccessful = FFileHelper::SaveStringToFile(saveText, *fullPath);

	UE_LOG(LogTemp, Warning, TEXT("%s"), isSuccessful ? *FString("Save Team result to File!") : *FString("Failed To Save..."));
}
