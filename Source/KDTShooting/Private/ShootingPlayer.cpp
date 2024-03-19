// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingPlayer.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

AShootingPlayer::AShootingPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// 1. 박스 컴포넌트를 생성한다.
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	
	// 1-1. 생성된 박스 컴포넌트를 루트 컴포넌트로 설정한다.
	SetRootComponent(boxComp);

	// 1-2. 박스 컴포넌트의 크기를 50x50x50으로 설정한다.
	boxComp->SetBoxExtent(FVector(50, 50, 50));

	// 2. 스태틱 메시 컴포넌트를 생성한다.
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));

	// 2-1. 생성된 메시 컴포넌트를 루트의 자식 컴포넌트로 설정한다.
	meshComp->SetupAttachment(boxComp);

	// 2-2. 메시 컴포넌트의 위치를 z축으로 -50만큼 내린다.
	meshComp->SetRelativeLocation(FVector(0, 0, -50));
}

void AShootingPlayer::BeginPlay()
{
	Super::BeginPlay();


}

void AShootingPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// 사용자가 입력한 방향대로 이동을 하고 싶다.



	//Move(FVector(0, 0, 1), DeltaTime);
}

void AShootingPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

void AShootingPlayer::Move(FVector direction, float deltaTime)
{
	// direction의 방향으로 이동한다.
	// 이동 구성 요소 : 방향, 속력, 시간
	FVector prevLocation = GetActorLocation();
	FVector nextLocation = prevLocation + direction * speed * deltaTime;
	SetActorLocation(nextLocation);

	//SetActorLocation(GetActorLocation() + direction * speed * deltaTime);
}


