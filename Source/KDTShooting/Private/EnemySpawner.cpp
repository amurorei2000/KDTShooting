// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Components/SceneComponent.h"
#include "EnemyActor.h"
#include "Components/ArrowComponent.h"


AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(rootComp);

	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Component"));
	arrowComp->SetupAttachment(rootComp);
	arrowComp->SetRelativeRotation(FRotator(-90, 0, 180));
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 지정된 시간마다 에너미를 생성한다.
	// 필요 변수: 시간(간격), 에너미 참조 변수

	// 프레임 시간을 currentTime 변수에 누적시킨다.
	currentTime += DeltaTime;

	if (currentTime > delayTime)
	{
		// 에너미 블루프린트를 생성한다.
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<AEnemyActor>(enemy_bp, arrowComp->GetComponentLocation(), arrowComp->GetComponentRotation(), params);

		// 누적된 시간을 0초로 초기화한다.
		currentTime = 0;
	}
}

