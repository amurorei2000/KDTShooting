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
	// �ڵ� ������ ���� �ʵ��� ����
	audioComp->SetAutoActivate(false);	
	
}

void ABGMActor::BeginPlay()
{
	Super::BeginPlay();

	// Kismet ����� �̿��ؼ� ������ ����� ��
	//UGameplayStatics::PlaySound2D(GetWorld(), bgm);

	// AudioComponent�� �̿��ؼ� ������ ����� ��
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
			// ����� ����
			audioComp->Stop();

			bIsOff = true;
		}
	}
}

