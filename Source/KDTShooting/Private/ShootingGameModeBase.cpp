// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGameModeBase.h"
#include "MainWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include "GameOverWidget.h"


void AShootingGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// 위젯 파일이 할당되어 있다면, 그 위젯 파일을 인스턴스로 생성한다.
	if (mainWidget_BP != nullptr)
	{
		mainUI = CreateWidget<UMainWidget>(GetWorld(), mainWidget_BP);

		// 위젯 파일이 제대로 생성되었다면, 위젯 인스턴스를 뷰포트에 추가한다.
		if (mainUI != nullptr)
		{
			mainUI->AddToViewport();

			// text_currentPoint 변수의 text 값을 "0"으로 초기화 해준다.
			mainUI->text_currentPoint->SetText(FText::FromString("0"));

			// 파일에 있는 최고 점수 값을 읽어서 bestPoint 변수에 넣는다.
			FString loadPoint;
			FString fullPath = FPaths::ProjectContentDir() + FString("/Save/BestScore.txt");
			FFileHelper::LoadFileToString(loadPoint, *fullPath);
			bestPoint = FCString::Atoi(*loadPoint);

			// bestPoint 변수의 값을 출력한다.
			mainUI->text_bestPoint->SetText(FText::FromString(FString::Printf(TEXT("%d"), bestPoint)));
		}
	}
}


void AShootingGameModeBase::AddPoint(int32 count)
{
	// 현재 점수에서 count만큼을 누적한다.
	point += count;

	// 만일, 현재 점수가 기존의 최고 점수를 초과했다면...
	if (point > bestPoint)
	{
		// 최고 점수를 현재 점수 값으로 갱신한다.
		bestPoint = point;

		// 갱신된 bestPoint 변수의 값을 위젯에 출력한다.
		mainUI->text_bestPoint->SetText(FText::FromString(FString::Printf(TEXT("%d"), bestPoint)));
	}

	// Main Widget의 text_currentPoint 변수의 text 항목 값에 현재 포인트 값을 넣는다.

	// 1. Main Widget을 찾아서 접근한다.(포인터 변수)
	if (mainUI != nullptr)
	{
		// 2. point 변수의 값을 FText 형태로 변환한다(int32 -> FText)
		FString pointString = FString::Printf(TEXT("%d"), point);
		FText pointText = FText::FromString(pointString);

		//FText pointText2 = FText::FromString(FString::Printf(TEXT("%d"), point));

		// 3. 찾은 위젯의 text_currentPoint변수의 멤버 변수인 text 항목에 접근해서 point 값을 넣는다.
		mainUI->text_currentPoint->SetText(pointText);
		
		// 위젯 애니메이션을 실행한다.
		mainUI->PlayAnimationForward(mainUI->anim_currentScore, 2);
	}
}

// 게임 오버 UI를 화면에 그리는 함수
void AShootingGameModeBase::ShowGameOverUI()
{
	if (gameOverWidget_BP != nullptr)
	{
		gameOverUI = CreateWidget<UGameOverWidget>(GetWorld(), gameOverWidget_BP);

		if (gameOverUI != nullptr)
		{
			// 게임 오버 UI를 화면에 그린다.
			gameOverUI->AddToViewport();

			// 마우스 커서를 화면에 보이게 처리한다.
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);

			// 메인 UI는 화면에서 제거한다.
			//mainUI->RemoveFromParent();

			// 메인 UI의 점수 표시를 화면에서 보이지 않게 숨겨놓는다.
			mainUI->ShowCurrentPointText(false);

			// 게임을 일시 정지 상태로 놓는다.
			// 월드의 프레임 시간 배율을 0으로 바꾼다.
			GetWorldSettings()->SetTimeDilation(0);
		}
	}
}

void AShootingGameModeBase::HideGameOverUI()
{
	// 월드의 시간 배율을 1배속으로 변경한다.
	GetWorldSettings()->SetTimeDilation(1.0f);

	// 마우스 커서를 안보이게 한다.
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);

	// 게임 오버 위젯을 제거한다.
	gameOverUI->RemoveFromParent();

	// 메인 UI 위젯을 다시 생성한다.
	// mainUI->AddToViewport();

	// 메인 UI의 현재 점수를 다시 화면에 보이게한다.
	mainUI->ShowCurrentPointText(true);
}

void AShootingGameModeBase::SaveBestScore()
{
	// 최고 점수를 파일(.txt)로 저장한다.
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
