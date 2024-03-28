// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "ShootingGameModeBase.h"

// 생성 후 처음 실행되는 라이프 사이클 함수(= BeginPlay)
void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 재시작 버튼의 클릭 이벤트에 RestartGame 함수를 연결한다.
	btn_restart->OnClicked.AddDynamic(this, &UGameOverWidget::RestartGame);

	// 종료 버튼의 클릭 이벤트에 QuitGame 함수를 연결한다.
	btn_quit->OnClicked.AddDynamic(this, &UGameOverWidget::QuitGame);

	btn_continue->OnClicked.AddDynamic(this, &UGameOverWidget::ContinueGame);
}

void UGameOverWidget::RestartGame()
{
	// Level을 다시 연다.
	UGameplayStatics::OpenLevel(GetWorld(), FName("ShootingMap"));
	
}

void UGameOverWidget::QuitGame()
{
	// 어플리케이션을 종료한다.
	//UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Type::Quit, true);
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("Quit");
}

void UGameOverWidget::ContinueGame()
{
	// 게임 모드에 있는 HideGameOverUI() 함수를 실행한다.
	AShootingGameModeBase* gm = Cast<AShootingGameModeBase>(GetWorld()->GetAuthGameMode());
	
	if (gm != nullptr)
	{
		gm->HideGameOverUI();
	}
}
