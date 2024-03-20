// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletActor.h"

ABulletActor::ABulletActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABulletActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

