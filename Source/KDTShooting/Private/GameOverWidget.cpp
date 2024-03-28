// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "ShootingGameModeBase.h"

// ���� �� ó�� ����Ǵ� ������ ����Ŭ �Լ�(= BeginPlay)
void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ����� ��ư�� Ŭ�� �̺�Ʈ�� RestartGame �Լ��� �����Ѵ�.
	btn_restart->OnClicked.AddDynamic(this, &UGameOverWidget::RestartGame);

	// ���� ��ư�� Ŭ�� �̺�Ʈ�� QuitGame �Լ��� �����Ѵ�.
	btn_quit->OnClicked.AddDynamic(this, &UGameOverWidget::QuitGame);

	btn_continue->OnClicked.AddDynamic(this, &UGameOverWidget::ContinueGame);
}

void UGameOverWidget::RestartGame()
{
	// Level�� �ٽ� ����.
	UGameplayStatics::OpenLevel(GetWorld(), FName("ShootingMap"));
	
}

void UGameOverWidget::QuitGame()
{
	// ���ø����̼��� �����Ѵ�.
	//UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Type::Quit, true);
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("Quit");
}

void UGameOverWidget::ContinueGame()
{
	// ���� ��忡 �ִ� HideGameOverUI() �Լ��� �����Ѵ�.
	AShootingGameModeBase* gm = Cast<AShootingGameModeBase>(GetWorld()->GetAuthGameMode());
	
	if (gm != nullptr)
	{
		gm->HideGameOverUI();
	}
}
