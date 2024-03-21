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

	// �ڽ� ������Ʈ�� �ֻ�� ������Ʈ�� ���� �� �����Ѵ�.
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50));

	// ����ƽ �޽� ������Ʈ�� �ڽ� ������Ʈ�� �ڽ� ������Ʈ�� ���� �� ����Ѵ�.
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	meshComp->SetupAttachment(boxComp);
	meshComp->SetRelativeLocation(FVector(0, 0, -50));

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
		FVector lookDirection = player->GetActorLocation() - GetActorLocation();
		moveDir = lookDirection.GetSafeNormal();
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

	return players[0];
}

