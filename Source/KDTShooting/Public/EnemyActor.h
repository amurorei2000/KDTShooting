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

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

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

	UPROPERTY(EditAnywhere, Category="MySettings")
	int32 itemRate = 50;

	UPROPERTY(EditAnywhere, Category="MySettings")
	TSubclassOf<class APowerItem> item_bp;

private:
	FVector moveDir;

	class AShootingPlayer* FindPlayer_BP();
	class AShootingPlayer* FindPlayer_Iterator();

	void CheckGenerateItem();

	UFUNCTION()
	void OnOverlapPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
