// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TeamDrawWidget.generated.h"

/**
 * 
 */
UCLASS()
class KDTSHOOTING_API UTeamDrawWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, Category="MySettings", meta=(BindWidget))
	class UWidgetSwitcher* sw_Steps;

	UPROPERTY(VisibleAnywhere, Category="MySettings", meta=(BindWidget))
	class UEditableText* edit_name1;

	UPROPERTY(VisibleAnywhere, Category="MySettings", meta=(BindWidget))
	class UEditableText* edit_name2;

	UPROPERTY(VisibleAnywhere, Category="MySettings", meta=(BindWidget))
	class UEditableText* edit_name3;

	UPROPERTY(VisibleAnywhere, Category="MySettings", meta=(BindWidget))
	class UEditableText* edit_name4;

	UPROPERTY(VisibleAnywhere, Category="MySettings", meta=(BindWidget))
	class UEditableText* edit_name5;

	UPROPERTY(VisibleAnywhere, Category="MySettings", meta=(BindWidget))
	class UEditableText* edit_name6;

	UPROPERTY(VisibleAnywhere, Category="MySettings", meta=(BindWidget))
	class UEditableText* edit_name7;

	UPROPERTY(VisibleAnywhere, Category="MySettings", meta=(BindWidget))
	class UButton* btn_Shuffle;

	UPROPERTY(VisibleAnywhere, Category="MySettings", meta=(BindWidget))
	class UTextBlock* text_shuffling;

	UPROPERTY(VisibleAnywhere, Category="MySettings", meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* anim_textShow;

	UPROPERTY(VisibleAnywhere, Category="MySettings", meta=(BindWidget))
	class UTextBlock* text_name1;

	UPROPERTY(VisibleAnywhere, Category="MySettings", meta=(BindWidget))
	class UTextBlock* text_name2;

	UPROPERTY(VisibleAnywhere, Category="MySettings", meta=(BindWidget))
	class UTextBlock* text_name3;

	UPROPERTY(VisibleAnywhere, Category="MySettings", meta=(BindWidget))
	class UTextBlock* text_name4;

	UPROPERTY(VisibleAnywhere, Category="MySettings", meta=(BindWidget))
	class UTextBlock* text_name5;

	UPROPERTY(VisibleAnywhere, Category="MySettings", meta=(BindWidget))
	class UTextBlock* text_name6;

	UPROPERTY(VisibleAnywhere, Category="MySettings", meta=(BindWidget))
	class UTextBlock* text_name7;

	UPROPERTY(VisibleAnywhere, Category="MySettings", meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* anim_resultShow;

protected:
	virtual void NativeConstruct() override;

private:
	TArray<FString> teams;
	TArray<FString> teamResults;


	UFUNCTION()
	void StartShuffle();

	UFUNCTION()
	void ShowTeamResults();
};
