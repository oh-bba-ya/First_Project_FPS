// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"


void AFPSGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (mainWidget != nullptr) {
		// mainWidget �������Ʈ ������ �޸𸮿� �ε��Ѵ�.
		mainUI = CreateWidget<UMainWidget>(GetWorld(), mainWidget);

		// ������ �޸𸮿� �ε�Ǹ� �� ��Ʈ�� ����Ѵ�.
		if (mainUI != nullptr) {
			mainUI->AddToViewport();
		}
	}

}

void AFPSGameModeBase::PrintScore()
{
	if (mainUI != nullptr) {
		// ScoreData �ؽ�Ʈ ��Ͽ� ���� ���� ���� �Է��Ѵ�.
		mainUI->scoreData->SetText(FText::AsNumber(currentScore));
	}
}

// ���� ������ ����ϴ� �Լ�
void AFPSGameModeBase::AddScore(int32 point)
{
	// �Ű����� point�� ���� �Ѱܹ��� ������ ���� ������ ������Ų��.
	currentScore += point;

	PrintScore();
}


