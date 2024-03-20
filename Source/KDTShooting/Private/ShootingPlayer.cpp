﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingPlayer.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"


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

	//UE_LOG(LogTemp, Warning, TEXT("Call BeginPlay Function"));

	// EnhacedInputSystem에서 입력 맵핑 콘텍스트 파일을 언리얼 엔진 시스템에 로드하기
	// 1. 현재 사용 중인 APlayerController 클래스 변수를 불러온다.
	APlayerController* pc = GetWorld()->GetFirstPlayerController();

	// 2. EnhancedInput 내용을 담은 Subsystem을 가져온다.
	if (pc != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());

		if (subsys != nullptr)
		{
			// 3. EnhancedInputLocalPlayerSubsystem에다가 imc 파일을 추가한다.
			subsys->AddMappingContext(imc_myMapping, 0);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Player Controller is Null!"));
	}

}

void AShootingPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// 사용자가 입력한 방향대로 이동을 하고 싶다.
	FVector moveDir = FVector(0, inputDir.X, inputDir.Y);

	Move(moveDir, DeltaTime);
}

void AShootingPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//UE_LOG(LogTemp, Warning, TEXT("Call SetupPlayerInputComponent Function"));

	// 입력 이벤트 컴포넌트에 실행할 함수를 연결(Binding)한다.
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (enhancedInputComponent != nullptr)
	{
		// 함수를 인풋 컴포넌트에 연결한다.
		enhancedInputComponent->BindAction(ia_move, ETriggerEvent::Triggered, this, &AShootingPlayer::SetInputDirection);
		enhancedInputComponent->BindAction(ia_move, ETriggerEvent::Completed, this, &AShootingPlayer::SetInputDirection);
	}
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

void AShootingPlayer::SetInputDirection(const FInputActionValue& value)
{
	// 사용자의 실제 입력 값을 inputDir 변수에 저장한다.
	inputDir = value.Get<FVector2D>();
}


