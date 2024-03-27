// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "Components/TextBlock.h"


void UMainWidget::ShowCurrentPointText(bool bShow)
{
	if (bShow)
	{
		text_currentPointText->SetVisibility(ESlateVisibility::Visible);
		text_currentPoint->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		text_currentPointText->SetVisibility(ESlateVisibility::Hidden);
		text_currentPoint->SetVisibility(ESlateVisibility::Hidden);
	}
}
