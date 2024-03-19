// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PointerTestActor.generated.h"

UCLASS()
class KDTSHOOTING_API APointerTestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APointerTestActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TArray<FString> teamNames;


	UFUNCTION()
	void StartTeamShuffle();
	void ShuffleTeams(const TArray<FString>& teams);

};
