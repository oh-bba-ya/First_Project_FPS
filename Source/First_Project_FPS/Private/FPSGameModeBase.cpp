// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "FPSGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "StartWidget.h"
#include "EnemyBoss.h"
#include "TimerManager.h"


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
	UE_LOG(LogTemp, Warning, TEXT("AddScore : %d"), currentScore);
	if (currentScore > bossScore) {
		if (!isSpawnBoss) {
			isSpawnBoss = true;
			FTimerHandle  spawnHandle;
			GetWorld()->GetTimerManager().SetTimer(spawnHandle, this, &AFPSGameModeBase::SpawnBoss,4,false);
		}
	}

	PrintScore();
}

void AFPSGameModeBase::ShowStart()
{
	
	if (startWidget != nullptr) {
		startUI = CreateWidget<UStartWidget>(GetWorld(), startWidget);

		if (startUI != nullptr) {
			// ������ ������ ����Ʈ�� ����Ѵ�.
			startUI->AddToViewport();

			// ������ �Ͻ����� ���·� �����.
			UGameplayStatics::SetGamePaused(GetWorld(), true);

			// ���콺 Ŀ�� ��Ÿ����
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
		}

	}

}

void AFPSGameModeBase::ShowEnd()
{
	if (startWidget != nullptr) {
		startUI = CreateWidget<UStartWidget>(GetWorld(), startWidget);

		if (startUI != nullptr) {
			FString text = FString(TEXT("Game Clear"));
			startUI->text_Title->SetText(FText::FromString(text));
			// ������ ������ ����Ʈ�� ����Ѵ�.
			startUI->AddToViewport();

			// ������ �Ͻ����� ���·� �����.
			UGameplayStatics::SetGamePaused(GetWorld(), true);

			// ���콺 Ŀ�� ��Ÿ����
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
		}

	}

}

void AFPSGameModeBase::SpawnBoss()
{


	GetWorld()->SpawnActor<AEnemyBoss>(bossActor, startLocation, FRotator::ZeroRotator);

}



