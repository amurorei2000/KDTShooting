// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"


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
}

void ABulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// p = p0 + vt
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * speed * DeltaTime);
}

// ������ �̺�Ʈ�� �߻��� �� ������ �Լ�
void ABulletActor::OnOverlapEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Collide Actor name: %s"), *OtherActor->GetActorNameOrLabel());

	// �浹�� ������ ��ġ�� ���� ȿ���� �����Ѵ�.
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionFX, OtherActor->GetActorLocation(), FRotator::ZeroRotator);

	// �ε��� ������ �����Ѵ�.
	OtherActor->Destroy();

	// ���� �����Ѵ�.
	Destroy();
}

