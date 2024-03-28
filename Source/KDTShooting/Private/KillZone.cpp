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
	
	// 에너미나 총알이 부딪히면 실행할 함수를 연결한다.
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AKillZone::Terminate);
}

void AKillZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 충돌 시 실행될 함수
void AKillZone::Terminate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 만일, 부딪힌 대상(OtherActor)의 클래스가 ABulletActor 클래스라면...
	//if (OtherActor->IsA<ABulletActor>())
	//{
		// "Bullet!!" 이라는 문구를 출력한다.
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

