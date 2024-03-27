// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShootingGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class KDTSHOOTING_API AShootingGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	int32 GetCurrentPoint();
	int32 GetBestPoint();

	UPROPERTY(EditAnywhere, Category ="MySettings")
	TSubclassOf<class UMainWidget> mainWidget_BP;

	UPROPERTY(EditAnywhere, Category ="MySettings")
	TSubclassOf<class UGameOverWidget> gameOverWidget_BP;


	void AddPoint(int32 count);
	void ShowGameOverUI();
	void HideGameOverUI();
	void SaveBestScore();

protected:
	virtual void BeginPlay() override;


private:
	int32 point = 0;
	int32 bestPoint = 0;
	class UMainWidget* mainUI;
	class UGameOverWidget* gameOverUI;
};
