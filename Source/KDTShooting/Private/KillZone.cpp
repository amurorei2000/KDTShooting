// Fill out your copyright notice in the Description page of Project Settings.


#include "KillZone.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "BulletActor.h"
#include "ShootingPlayer.h"


AKillZone::AKillZone()
{
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(rootComp);

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	boxComp->SetupAttachment(rootComp);
	boxComp->SetCollisionProfileName(FName("KillZonePreset"));
	boxComp->SetGenerateOverlapEvents(true);
}

void AKillZone::BeginPlay()
{
	Super::BeginPlay();
	
	// ���ʹ̳� �Ѿ��� �ε����� ������ �Լ��� �����Ѵ�.
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AKillZone::Terminate);
}

void AKillZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// �浹 �� ����� �Լ�
void AKillZone::Terminate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ����, �ε��� ���(OtherActor)�� Ŭ������ ABulletActor Ŭ�������...
	//if (OtherActor->IsA<ABulletActor>())
	//{
		// "Bullet!!" �̶�� ������ ����Ѵ�.
		//UE_LOG(LogTemp, Warning, TEXT("Bullet!!"));
	//}

	if (OtherActor->IsA<ABulletActor>())
	{
		ABulletActor* bullet = Cast<ABulletActor>(OtherActor);
		bullet->BulletActivate(false);
	}
	else if (!OtherActor->IsA<AShootingPlayer>())
	{
		OtherActor->Destroy();
	}
	
}

