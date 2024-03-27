// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class KDTSHOOTING_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(VisibleAnywhere, Category="MySettings", meta=(BindWidget))
	class UButton* btn_restart;

	UPROPERTY(VisibleAnywhere, Category="MySettings", meta=(BindWidget))
	class UButton* btn_quit;

	UPROPERTY(VisibleAnywhere, Category="MySettings", meta=(BindWidget))
	class UButton* btn_continue;


private:
	UFUNCTION()
	void RestartGame();

	UFUNCTION()
	void QuitGame();

	UFUNCTION()
	void ContinueGame();
};
