// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class KDTSHOOTING_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category="MySettings", meta=(BindWidget))
	class UTextBlock* text_currentPoint;

	UPROPERTY(EditAnywhere, Category="MySettings", meta=(BindWidget))
	class UTextBlock* text_currentPointText;

	UPROPERTY(EditAnywhere, Category="MySettings", meta=(BindWidget))
	class UTextBlock* text_bestPoint;

	UPROPERTY(EditAnywhere, Category="MySettings", meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* anim_currentScore;

	void ShowCurrentPointText(bool bShow);
};
