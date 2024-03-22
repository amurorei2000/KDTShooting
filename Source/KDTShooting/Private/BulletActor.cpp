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

	// 1. 콜리전 설정을 직접 수정할 때
	// 박스의 콜리전은 충돌 시 영역 감지만 사용하도록 설정한다.(물리효과 X)
	//boxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// 콜리전의 오브젝트 타입을 GameTraceChannel1(Bullet)로 설정한다.
	//boxComp->SetCollisionObjectType(ECC_GameTraceChannel1);

	// 다른 콜리전과 충돌했을 때의 응답을 일괄적으로 Ignore로 설정한다.
	//boxComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	// GameTraceChannel2(Enemy)와 충돌했을 때의 응답을 Overlap으로 설정한다.
	//boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);

	// 2. 콜리전 설정을 프리셋을 이용해서 수정할 때
	boxComp->SetCollisionProfileName(FName("BulletPreset"));

	// 박스 콜리전의 오버랩 충돌 이벤트를 받을 수 있는 상태로 한다.
	boxComp->SetGenerateOverlapEvents(true);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	meshComp->SetupAttachment(boxComp);
	meshComp->SetRelativeLocation(FVector(0, 0, -50));
	// 메시의 콜리전은 사용하지 않도록 처리한다.
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABulletActor::BeginPlay()
{
	Super::BeginPlay();
	
	// 오버랩 충돌 시에 발생할 이벤트와 함수를 연결한다.
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ABulletActor::OnOverlapEnemy);
}

void ABulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// p = p0 + vt
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * speed * DeltaTime);
}

// 오버랩 이벤트가 발생할 때 실행할 함수
void ABulletActor::OnOverlapEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Collide Actor name: %s"), *OtherActor->GetActorNameOrLabel());

	// 충돌한 상대방의 위치에 폭발 효과를 실행한다.
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionFX, OtherActor->GetActorLocation(), FRotator::ZeroRotator);

	// 부딪힌 상대방을 제거한다.
	OtherActor->Destroy();

	// 나도 제거한다.
	Destroy();
}

