// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerItem.generated.h"

UCLASS()
class KDTSHOOTING_API APowerItem : public AActor
{
	GENERATED_BODY()
	
public:	
	APowerItem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category="MySettings")
	class UBoxComponent* boxComp;

	UPROPERTY(VisibleAnywhere, Category="MySettings")
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, Category="MySettings")
	float moveSpeed = 100;
};
