// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BGMActor.generated.h"

UCLASS()
class KDTSHOOTING_API ABGMActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABGMActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category="MySettings")
	class USceneComponent* rootComp;

	UPROPERTY(VisibleAnywhere, Category="MySettings")
	class UAudioComponent* audioComp;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class USoundBase* bgm;

	float currentTime = 0;
	float offTime = 3.0f;

	bool bIsOff = false;
};
