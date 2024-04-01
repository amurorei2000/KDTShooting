// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingPlayer.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "BulletActor.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ShootingGameModeBase.h"
#include "PowerItem.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>


AShootingPlayer::AShootingPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// 1. 박스 컴포넌트를 생성한다.
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));

	// 1-1. 생성된 박스 컴포넌트를 루트 컴포넌트로 설정한다.
	SetRootComponent(boxComp);

	// 1-2. 박스 컴포넌트의 크기를 50x50x50으로 설정한다.
	boxComp->SetBoxExtent(FVector(50, 50, 50));

	boxComp->SetCollisionProfileName(FName("PlayerPreset"));


	// 2. 스태틱 메시 컴포넌트를 생성한다.
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));

	// 2-1. 생성된 메시 컴포넌트를 루트의 자식 컴포넌트로 설정한다.
	meshComp->SetupAttachment(boxComp);

	// 2-2. 메시 컴포넌트의 위치를 z축으로 -50만큼 내린다.
	//meshComp->SetRelativeLocation(FVector(0, 0, -50));
	meshComp->SetRelativeRotation(FRotator(-90, 180, 0));
	meshComp->SetRelativeScale3D(FVector(5.0f));
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 2-3. Static Mesh 파일을 StaticMeshComponent의 mesh 속성 값으로 할당한다.
	// 리소스 파일 로드: ConstructorHelpers::FOjectFinder<클래스명> 변수명(TEXT("소스 경로"))
	static ConstructorHelpers::FObjectFinder<UStaticMesh> playerMesh(TEXT("/Script/Engine.StaticMesh'/Game/Mesh/Spaceship_ARA.Spaceship_ARA'"));

	// 리소스 파일을 로드하는데 성공했다면...
	if (playerMesh.Succeeded())
	{
		// 해당 오브젝트 값을 컴포넌트에 할당한다.
		meshComp->SetStaticMesh(playerMesh.Object);
	}


	// 3. 총구 표시용 화살표 컴포넌트를 생성한다.
	fireLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Component"));
	fireLocation->SetupAttachment(boxComp);
	fireLocation->SetRelativeLocation(FVector(0, 0, 100));
	fireLocation->SetRelativeRotation(FRotator(90, 0, 0));

	// 총알 블루프린트 파일(Class)을 bulletFactory 변수에 할당한다.
	// 주의: 블루프린트 파일 경로(Find Reference)의 끝에 _C를 붙여준다.
	static ConstructorHelpers::FClassFinder<ABulletActor> bulletBP(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_BulletActor.BP_BulletActor_C'"));

	if (bulletBP.Succeeded())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Bullet Load Success!"));
		bulletFactory = bulletBP.Class;
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Bullet Load Failed!"));
	}
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

	// poolSize 수 만큼 총알(ABulletActor)을 생성한다.
	// 1. poolsize만큼 반복한다.
	for (int32 i = 0; i < poolSize; i++)
	{
		// 2. 총알 인스턴스를 생성한다.
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ABulletActor* bullet = GetWorld()->SpawnActor<ABulletActor>(bulletFactory, FVector::ZeroVector, FRotator::ZeroRotator, params);

		// 3. 총알을 비활성화한다.
		bullet->BulletActivate(false);

		// 4. 총알을 배열에 넣는다.
		bulletPool.Add(bullet);

		// 5. 총알에 있는 player 변수에 자기 자신을 참조시킨다.
		bullet->player = this;
	}

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AShootingPlayer::OnOverlapItem);
	boxComp->OnComponentEndOverlap.AddDynamic(this, &AShootingPlayer::OnEndOverlap);

}

void AShootingPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

	// 사용자가 입력한 방향대로 이동을 하고 싶다.
	FVector moveDir = FVector(0, inputDir.X, inputDir.Y);

	Move(moveDir, DeltaTime);

	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor(255, 0, 0), FString("Test"));

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
		enhancedInputComponent->BindAction(ia_fire, ETriggerEvent::Started, this, &AShootingPlayer::Fire);
		enhancedInputComponent->BindAction(ia_openMenu, ETriggerEvent::Started, this, &AShootingPlayer::ShowMenu);
	}
}

void AShootingPlayer::SetBulletPool(class ABulletActor* bullet)
{
	bulletPool.Add(bullet);
}

void AShootingPlayer::Move(FVector direction, float deltaTime)
{
	// direction의 방향으로 이동한다.
	// 이동 구성 요소 : 방향, 속력, 시간
	FVector prevLocation = GetActorLocation();
	FVector nextLocation = prevLocation + direction * speed * deltaTime;

	// sweep 체크를 하면서 이동한다.
	SetActorLocation(nextLocation, true);

	//SetActorLocation(GetActorLocation() + direction * speed * deltaTime);
}

void AShootingPlayer::SetInputDirection(const FInputActionValue& value)
{
	// 사용자의 실제 입력 값을 inputDir 변수에 저장한다.
	inputDir = value.Get<FVector2D>();
}

void AShootingPlayer::Fire(const FInputActionValue& value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Click Fire!!!"));

	// 만일, bCanFire 값이 false라면 함수를 중단한다.
	if (!bCanFire)
	{
		return;
	}

	// fireCount 수만큼 생성한다. = 반복
	for (int32 i = 0; i < fireCount; i++)
	{
		// 1. Object pooling 기법을 사용하지 않을 때
		// 충돌 옵션 : 무조건 내가 설정한 위치에서 생성되어야 한다.
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// 총알 액터를 위쪽에 생성(Spawn)한다.

		FVector firePos = fireLocation->GetComponentLocation();
		float totalDistance = interval * (fireCount - 1);
		float startLocation = firePos.Y + (totalDistance * -0.5f);
		firePos.Y = startLocation + interval * i;

		GetWorld()->SpawnActor<ABulletActor>(bulletFactory, firePos, fireLocation->GetComponentRotation(), params);
	}

	// 2. Object pooling 기법을 사용했을 때
	// bulletPool에 총알이 1발 이상 있는 것을 확인한다.
	//if (bulletPool.Num() > 0)
	//{
	//	// 배열의 첫 번째 총알을 임시 변수로 받아놓는다.
	//	ABulletActor* bullet = bulletPool[0];

	//	// 꺼낸 총알의 위치를 fireLocation에 놓는다.
	//	// 1. ArrowComponent 이용 시
	//	//bullet->SetActorLocation(fireLocation->GetComponentLocation());
	//	//bullet->SetActorRotation(fireLocation->GetComponentRotation());

	//	// 2. Socket을 이용할 시
	//	bullet->SetActorLocation(meshComp->GetSocketLocation(FName("FireSocket")));
	//	bullet->SetActorRotation(meshComp->GetSocketRotation(FName("FireSocket")));

	//	// 총알을 활성화한다.
	//	bullet->BulletActivate(true);

	//	// 배열의 첫 번째 요소를 제거한다.
	//	bulletPool.RemoveAt(0);


	//	// 총알 효과음을 출력한다.
	//	if (fireSound != nullptr)
	//	{
	//		UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
	//	}
	//}
}

void AShootingPlayer::FireCircle(const FInputActionValue& value)
{
	for (int32 i = 0; i < fireCount; i++)
	{
		// 간격(각도) = 360 / fireCount
		float degree = 360.0f / (float)fireCount;
		float radian = FMath::DegreesToRadians(degree * i);

		float yValue = FMath::Cos(radian) * 200;
		float zValue = FMath::Sin(radian) * 200;

		FVector circleVec = FVector(0, yValue, zValue);
		FRotator newRot = UKismetMathLibrary::MakeRotFromZX(fireLocation->GetUpVector(), circleVec);


		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<ABulletActor>(bulletFactory, GetActorLocation() + circleVec, newRot, params);
	}
}

void AShootingPlayer::ShowMenu(const FInputActionValue& value)
{
	// 게임 모드에 있는 ShowGameOverUI() 함수를 실행한다.
	AShootingGameModeBase* gm = Cast<AShootingGameModeBase>(GetWorld()->GetAuthGameMode());

	if (gm != nullptr)
	{
		gm->ShowGameOverUI();
	}
}

void AShootingPlayer::OnOverlapItem(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APowerItem* item = Cast<APowerItem>(OtherActor);

	if (item != nullptr)
	{
		// 충돌한 대상이 item이라면 fireCount를 1 증가시킨다.
		fireCount++;

		// fireCount의 값이 maxFireCount를 넘기지 못하게 한다.
		/*if (fireCount > maxFireCount)
		{
			fireCount = maxFireCount;
		}*/

		fireCount = FMath::Min(fireCount, maxFireCount);

		// item을 제거한다.
		item->Destroy();
	}
	else
	{
		// 충돌한 액터의 이름에 "Trap"이라는 글자가 포함되어 있다면...
		if (OtherActor->GetActorNameOrLabel().Contains("Trap"))
		{
			bCanFire = false;
		}
	}

}

void AShootingPlayer::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->GetActorNameOrLabel().Contains("Trap"))
	{
		bCanFire = true;
	}
}

