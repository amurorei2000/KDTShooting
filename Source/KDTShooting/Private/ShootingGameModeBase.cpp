// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGameModeBase.h"
#include "MainWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include "GameOverWidget.h"


void AShootingGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// ���� ������ �Ҵ�Ǿ� �ִٸ�, �� ���� ������ �ν��Ͻ��� �����Ѵ�.
	if (mainWidget_BP != nullptr)
	{
		mainUI = CreateWidget<UMainWidget>(GetWorld(), mainWidget_BP);

		// ���� ������ ����� �����Ǿ��ٸ�, ���� �ν��Ͻ��� ����Ʈ�� �߰��Ѵ�.
		if (mainUI != nullptr)
		{
			mainUI->AddToViewport();

			// text_currentPoint ������ text ���� "0"���� �ʱ�ȭ ���ش�.
			mainUI->text_currentPoint->SetText(FText::FromString("0"));
		}
	}

	
}


void AShootingGameModeBase::AddPoint(int32 count)
{
	// ���� �������� count��ŭ�� �����Ѵ�.
	point += count;

	// Main Widget�� text_currentPoint ������ text �׸� ���� ���� ����Ʈ ���� �ִ´�.

	// 1. Main Widget�� ã�Ƽ� �����Ѵ�.(������ ����)
	if (mainUI != nullptr)
	{
		// 2. point ������ ���� FText ���·� ��ȯ�Ѵ�(int32 -> FText)
		FString pointString = FString::Printf(TEXT("%d"), point);
		FText pointText = FText::FromString(pointString);

		//FText pointText2 = FText::FromString(FString::Printf(TEXT("%d"), point));

		// 3. ã�� ������ text_currentPoint������ ��� ������ text �׸� �����ؼ� point ���� �ִ´�.
		mainUI->text_currentPoint->SetText(pointText);

	}
}

// ���� ���� UI�� ȭ�鿡 �׸��� �Լ�
void AShootingGameModeBase::ShowGameOverUI()
{
	if (gameOverWidget_BP != nullptr)
	{
		gameOverUI = CreateWidget<UGameOverWidget>(GetWorld(), gameOverWidget_BP);

		if (gameOverUI != nullptr)
		{
			gameOverUI->AddToViewport();

			// ���� UI�� ȭ�鿡�� �����Ѵ�.
			mainUI->RemoveFromParent();
		}
	}
}

int32 AShootingGameModeBase::GetCurrentPoint()
{
	return point;
}
