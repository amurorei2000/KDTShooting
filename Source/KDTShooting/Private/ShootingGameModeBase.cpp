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

			// ���Ͽ� �ִ� �ְ� ���� ���� �о bestPoint ������ �ִ´�.
			FString loadPoint;
			FString fullPath = FPaths::ProjectContentDir() + FString("/Save/BestScore.txt");
			FFileHelper::LoadFileToString(loadPoint, *fullPath);
			bestPoint = FCString::Atoi(*loadPoint);

			// bestPoint ������ ���� ����Ѵ�.
			mainUI->text_bestPoint->SetText(FText::FromString(FString::Printf(TEXT("%d"), bestPoint)));
		}
	}
}


void AShootingGameModeBase::AddPoint(int32 count)
{
	// ���� �������� count��ŭ�� �����Ѵ�.
	point += count;

	// ����, ���� ������ ������ �ְ� ������ �ʰ��ߴٸ�...
	if (point > bestPoint)
	{
		// �ְ� ������ ���� ���� ������ �����Ѵ�.
		bestPoint = point;

		// ���ŵ� bestPoint ������ ���� ������ ����Ѵ�.
		mainUI->text_bestPoint->SetText(FText::FromString(FString::Printf(TEXT("%d"), bestPoint)));
	}

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
		
		// ���� �ִϸ��̼��� �����Ѵ�.
		mainUI->PlayAnimationForward(mainUI->anim_currentScore, 2);
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
			// ���� ���� UI�� ȭ�鿡 �׸���.
			gameOverUI->AddToViewport();

			// ���콺 Ŀ���� ȭ�鿡 ���̰� ó���Ѵ�.
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);

			// ���� UI�� ȭ�鿡�� �����Ѵ�.
			//mainUI->RemoveFromParent();

			// ���� UI�� ���� ǥ�ø� ȭ�鿡�� ������ �ʰ� ���ܳ��´�.
			mainUI->ShowCurrentPointText(false);

			// ������ �Ͻ� ���� ���·� ���´�.
			// ������ ������ �ð� ������ 0���� �ٲ۴�.
			GetWorldSettings()->SetTimeDilation(0);
		}
	}
}

void AShootingGameModeBase::HideGameOverUI()
{
	// ������ �ð� ������ 1������� �����Ѵ�.
	GetWorldSettings()->SetTimeDilation(1.0f);

	// ���콺 Ŀ���� �Ⱥ��̰� �Ѵ�.
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);

	// ���� ���� ������ �����Ѵ�.
	gameOverUI->RemoveFromParent();

	// ���� UI ������ �ٽ� �����Ѵ�.
	// mainUI->AddToViewport();

	// ���� UI�� ���� ������ �ٽ� ȭ�鿡 ���̰��Ѵ�.
	mainUI->ShowCurrentPointText(true);
}

void AShootingGameModeBase::SaveBestScore()
{
	// �ְ� ������ ����(.txt)�� �����Ѵ�.
	FString savePoint = FString::Printf(TEXT("%d"), bestPoint);
	FString fullPath = FPaths::ProjectContentDir() + FString("/Save/BestScore.txt");

	FFileHelper::SaveStringToFile(savePoint, *fullPath);
}

int32 AShootingGameModeBase::GetCurrentPoint()
{
	return point;
}

int32 AShootingGameModeBase::GetBestPoint()
{
	return bestPoint;
}
