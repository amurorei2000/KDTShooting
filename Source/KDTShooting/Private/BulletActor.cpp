// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyActor.h"
#include "EngineUtils.h"
#include "ShootingGameModeBase.h"


ABulletActor::ABulletActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50));
	boxComp->SetWorldScale3D(FVector(0.7f, 0.3f, 1.0f));

	// 1. �ݸ��� ������ ���� ������ ��
	// �ڽ��� �ݸ����� �浹 �� ���� ������ ����ϵ��� �����Ѵ�.(����ȿ�� X)
	//boxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// �ݸ����� ������Ʈ Ÿ���� GameTraceChannel1(Bullet)�� �����Ѵ�.
	//boxComp->SetCollisionObjectType(ECC_GameTraceChannel1);

	// �ٸ� �ݸ����� �浹���� ���� ������ �ϰ������� Ignore�� �����Ѵ�.
	//boxComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	// GameTraceChannel2(Enemy)�� �浹���� ���� ������ Overlap���� �����Ѵ�.
	//boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);

	// 2. �ݸ��� ������ �������� �̿��ؼ� ������ ��
	boxComp->SetCollisionProfileName(FName("BulletPreset"));

	// �ڽ� �ݸ����� ������ �浹 �̺�Ʈ�� ���� �� �ִ� ���·� �Ѵ�.
	boxComp->SetGenerateOverlapEvents(true);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	meshComp->SetupAttachment(boxComp);
	meshComp->SetRelativeLocation(FVector(0, 0, -50));
	// �޽��� �ݸ����� ������� �ʵ��� ó���Ѵ�.
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABulletActor::BeginPlay()
{
	Super::BeginPlay();
	
	// ������ �浹 �ÿ� �߻��� �̺�Ʈ�� �Լ��� �����Ѵ�.
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ABulletActor::OnOverlapEnemy);
	
	//SetLifeSpan(5.0f);

	// ī�޶��� ��ġ�� �о�´�.
	for (TActorIterator<AActor> iter(GetWorld()); iter; ++iter)
	{
		AActor* foundActor = *iter;

		if (foundActor->GetActorNameOrLabel().Contains("Camera"))
		{
			cameraLocation = foundActor->GetActorLocation();
		}
	}
}

void ABulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// p = p0 + vt
	FVector targetLoc = GetActorLocation() + GetActorForwardVector() * speed * DeltaTime;

	// ����, targetLoc�� z�� ���� +1000 �����̰ų�, -1000 �̻��� ��
	if (targetLoc.Z <= cameraLocation.Z + 1000 && targetLoc.Z >= cameraLocation.Z -1000)
	{
		SetActorLocation(targetLoc);
	}
	else
	{
		Destroy();
	}
}

// ������ �̺�Ʈ�� �߻��� �� ������ �Լ�
void ABulletActor::OnOverlapEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Collide Actor name: %s"), *OtherActor->GetActorNameOrLabel());

	// �浹�� ������ ��ġ�� ���� ȿ���� �����Ѵ�.
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionFX, OtherActor->GetActorLocation(), FRotator::ZeroRotator);
	
	// ���� ȿ������ ����Ѵ�.
	if (explosionSound != nullptr)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), explosionSound);
	}

	// �ε��� ����� AEnemyActor Ŭ������� �� ����� �����Ѵ�.
	if (OtherActor->IsA<AEnemyActor>())
	{
		OtherActor->Destroy();

		// ���� ��� Ŭ������ ������ 1�� �߰��Ѵ�.
		// 1. ���� ��� Ŭ������ �����Ѵ�.
		AGameModeBase* gm = GetWorld()->GetAuthGameMode();
		AShootingGameModeBase* myGM = Cast<AShootingGameModeBase>(gm);

		if (myGM != nullptr)
		{
			// 2. AddPoint �Լ��� ȣ���Ѵ�.
			// 2-1. �Լ� ȣ�� �ÿ� �Ű������� 1�̶�� ���� �����Ѵ�.
			myGM->AddPoint(1);

			// �߰��� ����(���� ����)�� ����غ���.
			UE_LOG(LogTemp, Warning, TEXT("Current Point: %d"), myGM->GetCurrentPoint());
		}

	}

	// ���� �����Ѵ�.
	Destroy();
}

