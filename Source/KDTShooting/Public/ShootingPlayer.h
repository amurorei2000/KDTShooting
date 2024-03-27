// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ShootingPlayer.generated.h"


UCLASS()
class KDTSHOOTING_API AShootingPlayer : public APawn
{
	GENERATED_BODY()

public:
	AShootingPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 충돌체 컴포넌트 변수
	UPROPERTY(VisibleAnywhere, Category="MySettings")
	class UBoxComponent* boxComp;

	// 스태틱 메시 컴포넌트 변수
	UPROPERTY(VisibleAnywhere, Category="MySettings")
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, Category="MySettings")
	float speed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MySettings")
	FVector2D inputDir;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UInputMappingContext* imc_myMapping;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UInputAction* ia_move;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UInputAction* ia_fire;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UInputAction* ia_openMenu;

	UPROPERTY(EditAnywhere, Category="MySettings")
	TSubclassOf<class ABulletActor> bulletFactory;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UArrowComponent* fireLocation;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class USoundBase* fireSound;


private:
	UFUNCTION(BlueprintCallable)
	void Move(FVector direction , float deltaTime);

	UFUNCTION()
	void SetInputDirection(const FInputActionValue& value);

	UFUNCTION()
	void Fire(const FInputActionValue& value);

	UFUNCTION()
	void ShowMenu(const FInputActionValue& value);
};

