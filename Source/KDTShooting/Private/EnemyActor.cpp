// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ShootingPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

AEnemyActor::AEnemyActor()
{
	PrimaryActorTick.bCanEverTick = true;

	// 박스 컴포넌트를 최상단 컴포넌트로 생성 및 설정한다.
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50));

	// 스테틱 메시 컴포넌트를 박스 컴포넌트의 자식 컴포넌트로 생성 및 등록한다.
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	meshComp->SetupAttachment(boxComp);
	meshComp->SetRelativeLocation(FVector(0, 0, -50));

}

void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();


	// 0~99 사이의 임의의 숫자를 뽑았을 때, 그 숫자가 rate보다 크면
	int32 num = FMath::RandRange(0, 99);

	if (num >= rate)
	{
		// 이동 방향을 정면 방향으로 한다.
		// moveDir 변수에 자신의 앞 방향을 넣는다.
		moveDir = GetActorForwardVector();
	}
	// 그렇지 않으면...
	else
	{
		// 이동 방향을 플레이어를 향한 방향으로 한다.
		// moveDir 변수에 플레이어를 향한 방향 벡터를 넣는다.
		// 플레이어 위치 - 나의 위치

		// 1. 찾은 액터들 중의 첫 번째 액터를 AShootingPlayer 클래스로 변환해서 player 변수에 넣는다.
		//AShootingPlayer* player = FindPlayer_BP();
		AShootingPlayer* player = FindPlayer_Iterator();

		// 2. 내가 플레이어를 바라보는 방향 벡터를 구해서 moveDir 변수에 넣는다.
		FVector lookDirection = player->GetActorLocation() - GetActorLocation();
		moveDir = lookDirection.GetSafeNormal();
	}

	//// 현재 월드의 모든 액터(AActor*)의 이름을 UE_LOG로 출력해보세요.
	//for (TActorIterator<AActor> iter(GetWorld()); iter; ++iter)
	//{
	//	AActor* foundActor = *iter;

	//	if (foundActor->GetName().Contains(FString("Player")))
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *foundActor->GetName());
	//		//UE_LOG(LogTemp, Warning, TEXT("DName: %s"), *foundActor->GetActorNameOrLabel());
	//	}
	//}
}

void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// moveDir의 방향과 moveSpeed 속력으로 이동한다.
	// p = p0 + vt
	SetActorLocation(GetActorLocation() + moveDir * moveSpeed * DeltaTime);
}

// GetAllActorOfClass 방식으로 찾기
AShootingPlayer* AEnemyActor::FindPlayer_BP()
{
	// 1. 플레이어의 월드 상의 위치를 찾는다.
	// 1-1. 현재 월드 데이터에서 AShootingPlayer 클래스로 된 모든 액터들을 찾는다.
	TArray<AActor*> players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShootingPlayer::StaticClass(), players);
	
	// 2. AShootingPlayer 클래스 형태로 변환해서 반환한다.
	return Cast<AShootingPlayer>(players[0]);
}

// iterator 방식으로 찾기
AShootingPlayer* AEnemyActor::FindPlayer_Iterator()
{
	// for (TActorIterator<찾으려는 클래스> 변수이름(GetWorld()) ; 변수이름 ; ++변수이름)

	TArray<AShootingPlayer*> players;

	for (TActorIterator<AShootingPlayer> player(GetWorld()); player; ++player)
	{
		players.Add(*player);
	}

	return players[0];
}

