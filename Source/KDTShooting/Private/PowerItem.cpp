// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerItem.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>

APowerItem::APowerItem()
{
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50));
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	boxComp->SetCollisionObjectType(ECC_WorldDynamic);
	boxComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(boxComp);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	meshComp->SetRelativeLocation(FVector(0, 0, -50));

}

void APowerItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void APowerItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	SetActorLocation(GetActorLocation() + GetActorUpVector() * -1 * moveSpeed * DeltaTime);
}

