// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGameModeBase.h"
#include "MainWidget.h"


void AShootingGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// ���� ������ �Ҵ�Ǿ� �ִٸ�, �� ���� ������ �ν��Ͻ��� �����Ѵ�.
	if (mainWidget_BP != nullptr)
	{
		UMainWidget* mainUI = CreateWidget<UMainWidget>(GetWorld(), mainWidget_BP);

		// ���� ������ ����� �����Ǿ��ٸ�, ���� �ν��Ͻ��� ����Ʈ�� �߰��Ѵ�.
		if (mainUI != nullptr)
		{
			mainUI->AddToViewport();
		}
	}
}


void AShootingGameModeBase::AddPoint(int32 count)
{
	// ���� �������� count��ŭ�� �����Ѵ�.
	point += count;

	// Main Widget�� text_currentPoint ������ text �׸� ���� ���� ����Ʈ ���� �ִ´�.

	// 1. Main Widget�� ã�Ƽ� �����Ѵ�.(������ ����)


	// 2. ã�� ������ text_currentPoint������ ��� ������ text �׸� �����Ѵ�.

	// 3. ������ text �׸� ������ ���� point ���� ������ �ִ´�.(int32 -> FText)

}


int32 AShootingGameModeBase::GetCurrentPoint()
{
	return point;
}
