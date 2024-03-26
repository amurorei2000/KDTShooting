// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ShootingPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "ShootingGameModeBase.h"

AEnemyActor::AEnemyActor()
{
	PrimaryActorTick.bCanEverTick = true;

	// 박스 컴포넌트를 최상단 컴포넌트로 생성 및 설정한다.
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50));

	// 박스 컴포넌트의 콜리전 설정
	// 응답 타입: Query 타입
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	// 오브젝트 타입(채널): Enemy
	boxComp->SetCollisionObjectType(ECC_GameTraceChannel2);
	
	// 응답 처리: Player, Bullet 채널에 대해서 overlap으로 설정한다. 다른 채널에 대해서는 ignore로 처리한다.
	boxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);
	boxComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);

	boxComp->SetGenerateOverlapEvents(true);
	
	// 히트 이벤트(Block) 발생을 가능하게 설정한다.
	//boxComp->SetNotifyRigidBodyCollision(true);


	// 스테틱 메시 컴포넌트를 박스 컴포넌트의 자식 컴포넌트로 생성 및 등록한다.
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	meshComp->SetupAttachment(boxComp);
	meshComp->SetRelativeLocation(FVector(0, 0, -50));
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
		if (player != nullptr)
		{
			FVector lookDirection = player->GetActorLocation() - GetActorLocation();
			moveDir = lookDirection.GetSafeNormal();
		}
		else
		{
			moveDir = GetActorForwardVector();
		}
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

	// 오버랩 이벤트 함수를 BoxComponent의 beginOverlap 델리게이트에 연결한다.
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor::OnOverlapPlayer);

	// 생존 시간을 5초로 한다.
	//SetLifeSpan(5.0f);
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

	// players 배열에 1개라도 들어간 값이 있다면 그 값들 중에 0번을 반환하고...
	if (players.Num() > 0)
	{
		return players[0];
	}
	// 아무것도 없으면 nullptr를 반환한다.
	else
	{
		return nullptr;
	}
}

void AEnemyActor::OnOverlapPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 만일, 부딪힌 대상이 플레이어라면...
	AShootingPlayer* player = Cast<AShootingPlayer>(OtherActor);

	if (player != nullptr)
	{
		// 플레이어를 제거한다.
		player->Destroy();

		// 게임 모드 클래스에 있는 ShowGameOverUI() 함수를 호출한다.
		AShootingGameModeBase* gm = Cast<AShootingGameModeBase>(GetWorld()->GetAuthGameMode());

		if (gm != nullptr)
		{
			gm->ShowGameOverUI();
		}

		// 나도 제거한다.
		Destroy();
	}
}

