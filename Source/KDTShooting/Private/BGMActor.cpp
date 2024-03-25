// Fill out your copyright notice in the Description page of Project Settings.


#include "BGMActor.h"
#include "Kismet/GameplayStatics.h"
#include <Components/SceneComponent.h>
#include "Components/AudioComponent.h"

ABGMActor::ABGMActor()
{
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(rootComp);

	audioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	audioComp->SetupAttachment(rootComp);
	// 자동 시작이 되지 않도록 설정
	audioComp->SetAutoActivate(false);	
	
}

void ABGMActor::BeginPlay()
{
	Super::BeginPlay();

	// Kismet 기능을 이용해서 음원을 재생할 때
	//UGameplayStatics::PlaySound2D(GetWorld(), bgm);

	// AudioComponent를 이용해서 음원을 재생할 때
	audioComp->Play();
}

void ABGMActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsOff)
	{
		currentTime += DeltaTime;

		if (currentTime > offTime)
		{
			// 오디오 끄기
			audioComp->Stop();

			bIsOff = true;
		}
	}
}

