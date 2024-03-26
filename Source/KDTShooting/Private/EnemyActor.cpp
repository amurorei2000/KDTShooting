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

	// �ڽ� ������Ʈ�� �ֻ�� ������Ʈ�� ���� �� �����Ѵ�.
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50));

	// �ڽ� ������Ʈ�� �ݸ��� ����
	// ���� Ÿ��: Query Ÿ��
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	// ������Ʈ Ÿ��(ä��): Enemy
	boxComp->SetCollisionObjectType(ECC_GameTraceChannel2);
	
	// ���� ó��: Player, Bullet ä�ο� ���ؼ� overlap���� �����Ѵ�. �ٸ� ä�ο� ���ؼ��� ignore�� ó���Ѵ�.
	boxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);
	boxComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);

	boxComp->SetGenerateOverlapEvents(true);
	
	// ��Ʈ �̺�Ʈ(Block) �߻��� �����ϰ� �����Ѵ�.
	//boxComp->SetNotifyRigidBodyCollision(true);


	// ����ƽ �޽� ������Ʈ�� �ڽ� ������Ʈ�� �ڽ� ������Ʈ�� ���� �� ����Ѵ�.
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	meshComp->SetupAttachment(boxComp);
	meshComp->SetRelativeLocation(FVector(0, 0, -50));
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();


	// 0~99 ������ ������ ���ڸ� �̾��� ��, �� ���ڰ� rate���� ũ��
	int32 num = FMath::RandRange(0, 99);

	if (num >= rate)
	{
		// �̵� ������ ���� �������� �Ѵ�.
		// moveDir ������ �ڽ��� �� ������ �ִ´�.
		moveDir = GetActorForwardVector();
	}
	// �׷��� ������...
	else
	{
		// �̵� ������ �÷��̾ ���� �������� �Ѵ�.
		// moveDir ������ �÷��̾ ���� ���� ���͸� �ִ´�.
		// �÷��̾� ��ġ - ���� ��ġ

		// 1. ã�� ���͵� ���� ù ��° ���͸� AShootingPlayer Ŭ������ ��ȯ�ؼ� player ������ �ִ´�.
		//AShootingPlayer* player = FindPlayer_BP();
		AShootingPlayer* player = FindPlayer_Iterator();

		// 2. ���� �÷��̾ �ٶ󺸴� ���� ���͸� ���ؼ� moveDir ������ �ִ´�.
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

	//// ���� ������ ��� ����(AActor*)�� �̸��� UE_LOG�� ����غ�����.
	//for (TActorIterator<AActor> iter(GetWorld()); iter; ++iter)
	//{
	//	AActor* foundActor = *iter;

	//	if (foundActor->GetName().Contains(FString("Player")))
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *foundActor->GetName());
	//		//UE_LOG(LogTemp, Warning, TEXT("DName: %s"), *foundActor->GetActorNameOrLabel());
	//	}
	//}

	// ������ �̺�Ʈ �Լ��� BoxComponent�� beginOverlap ��������Ʈ�� �����Ѵ�.
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor::OnOverlapPlayer);

	// ���� �ð��� 5�ʷ� �Ѵ�.
	//SetLifeSpan(5.0f);
}

void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// moveDir�� ����� moveSpeed �ӷ����� �̵��Ѵ�.
	// p = p0 + vt
	SetActorLocation(GetActorLocation() + moveDir * moveSpeed * DeltaTime);
}

// GetAllActorOfClass ������� ã��
AShootingPlayer* AEnemyActor::FindPlayer_BP()
{
	// 1. �÷��̾��� ���� ���� ��ġ�� ã�´�.
	// 1-1. ���� ���� �����Ϳ��� AShootingPlayer Ŭ������ �� ��� ���͵��� ã�´�.
	TArray<AActor*> players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShootingPlayer::StaticClass(), players);
	
	// 2. AShootingPlayer Ŭ���� ���·� ��ȯ�ؼ� ��ȯ�Ѵ�.
	return Cast<AShootingPlayer>(players[0]);
}

// iterator ������� ã��
AShootingPlayer* AEnemyActor::FindPlayer_Iterator()
{
	// for (TActorIterator<ã������ Ŭ����> �����̸�(GetWorld()) ; �����̸� ; ++�����̸�)

	TArray<AShootingPlayer*> players;

	for (TActorIterator<AShootingPlayer> player(GetWorld()); player; ++player)
	{
		players.Add(*player);
	}

	// players �迭�� 1���� �� ���� �ִٸ� �� ���� �߿� 0���� ��ȯ�ϰ�...
	if (players.Num() > 0)
	{
		return players[0];
	}
	// �ƹ��͵� ������ nullptr�� ��ȯ�Ѵ�.
	else
	{
		return nullptr;
	}
}

void AEnemyActor::OnOverlapPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ����, �ε��� ����� �÷��̾���...
	AShootingPlayer* player = Cast<AShootingPlayer>(OtherActor);

	if (player != nullptr)
	{
		// �÷��̾ �����Ѵ�.
		player->Destroy();

		// ���� ��� Ŭ������ �ִ� ShowGameOverUI() �Լ��� ȣ���Ѵ�.
		AShootingGameModeBase* gm = Cast<AShootingGameModeBase>(GetWorld()->GetAuthGameMode());

		if (gm != nullptr)
		{
			gm->ShowGameOverUI();
		}

		// ���� �����Ѵ�.
		Destroy();
	}
}

