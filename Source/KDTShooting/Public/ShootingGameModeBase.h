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

	UPROPERTY(EditAnywhere, Category ="MySettings")
	TSubclassOf<class UMainWidget> mainWidget_BP;

	void AddPoint(int32 count);

protected:
	virtual void BeginPlay() override;


private:
	int32 point = 0;


};
