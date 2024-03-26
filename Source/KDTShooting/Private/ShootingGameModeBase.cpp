// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGameModeBase.h"
#include "MainWidget.h"


void AShootingGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// 위젯 파일이 할당되어 있다면, 그 위젯 파일을 인스턴스로 생성한다.
	if (mainWidget_BP != nullptr)
	{
		UMainWidget* mainUI = CreateWidget<UMainWidget>(GetWorld(), mainWidget_BP);

		// 위젯 파일이 제대로 생성되었다면, 위젯 인스턴스를 뷰포트에 추가한다.
		if (mainUI != nullptr)
		{
			mainUI->AddToViewport();
		}
	}
}


void AShootingGameModeBase::AddPoint(int32 count)
{
	// 현재 점수에서 count만큼을 누적한다.
	point += count;

	// Main Widget의 text_currentPoint 변수의 text 항목 값에 현재 포인트 값을 넣는다.

	// 1. Main Widget을 찾아서 접근한다.(포인터 변수)


	// 2. 찾은 위젯의 text_currentPoint변수의 멤버 변수인 text 항목에 접근한다.

	// 3. 접근한 text 항목 변수의 값을 point 변수 값으로 넣는다.(int32 -> FText)

}


int32 AShootingGameModeBase::GetCurrentPoint()
{
	return point;
}
