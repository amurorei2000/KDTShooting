// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyActor.generated.h"

UCLASS()
class KDTSHOOTING_API AEnemyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemyActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category="MySettings")
	class UBoxComponent* boxComp;

	UPROPERTY(VisibleAnywhere, Category="MySettings")
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, Category="MySettings")
	float moveSpeed = 750.0f;

	UPROPERTY(EditAnywhere, Category="MySettings")
	int32 rate = 60;

private:
	FVector moveDir;

	class AShootingPlayer* FindPlayer_BP();
	class AShootingPlayer* FindPlayer_Iterator();
};
