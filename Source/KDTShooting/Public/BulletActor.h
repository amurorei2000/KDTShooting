// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletActor.generated.h"



UCLASS()
class KDTSHOOTING_API ABulletActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABulletActor();


protected:
	virtual void BeginPlay() override;


public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, Category="MySettings")
	float speed = 1000;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UParticleSystem* explosionFX;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class USoundBase* explosionSound;

	class AShootingPlayer* player;


	void BulletActivate(bool bActivate);

private:
	UFUNCTION()
	void OnOverlapEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FVector cameraLocation;
};
