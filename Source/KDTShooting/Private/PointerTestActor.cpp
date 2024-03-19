// Fill out your copyright notice in the Description page of Project Settings.


#include "PointerTestActor.h"

// Sets default values
APointerTestActor::APointerTestActor()
{
	PrimaryActorTick.bCanEverTick = false;

}

void APointerTestActor::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle showTimer;

	GetWorldTimerManager().SetTimer(showTimer, this, &APointerTestActor::StartTeamShuffle, 3.0f, false);
}

void APointerTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APointerTestActor::StartTeamShuffle()
{
	ShuffleTeams(teamNames);
}

void APointerTestActor::ShuffleTeams(const TArray<FString>& teams)
{
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

	for (const FString& teamName : result)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s\n"), *teamName);
	}
}

