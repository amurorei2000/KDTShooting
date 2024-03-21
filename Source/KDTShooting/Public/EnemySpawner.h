// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class KDTSHOOTING_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawner();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category="MySettings")
	class USceneComponent* rootComp;

	UPROPERTY(VisibleAnywhere, Category="MySettings")
	class UArrowComponent* arrowComp;

	UPROPERTY(EditAnywhere, Category="MySettings")
	float delayTime = 2.0f;

	UPROPERTY(EditAnywhere, Category="MySettings")
	TSubclassOf<class AEnemyActor> enemy_bp;

private:
	float currentTime = 0;

};
