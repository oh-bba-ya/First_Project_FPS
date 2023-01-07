// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "FPSGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "StartWidget.h"


void AFPSGameModeBase::BeginPlay()
{
	//Super::BeginPlay();

	player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (mainWidget != nullptr) {
		// mainWidget 블루프린트 파일을 메모리에 로드한다.
		mainUI = CreateWidget<UMainWidget>(GetWorld(), mainWidget);

		// 위젯이 메모리에 로드되면 뷰 포트에 출력한다.
		if (mainUI != nullptr) {
			mainUI->AddToViewport();
		}
	}
}

void AFPSGameModeBase::PrintScore()
{
	if (mainUI != nullptr) {
		// ScoreData 텍스트 블록에 현재 점수 값을 입력한다.
		mainUI->scoreData->SetText(FText::AsNumber(currentScore));
	}
}

// 현재 점수를 계산하는 함수
void AFPSGameModeBase::AddScore(int32 point)
{
	// 매개변수 point를 통해 넘겨받은 점수를 현재 점수에 누적시킨다.
	currentScore += point;

	PrintScore();
}

void AFPSGameModeBase::ShowStart()
{
	
	if (startWidget != nullptr) {
		startUI = CreateWidget<UStartWidget>(GetWorld(), startWidget);

		if (startUI != nullptr) {
			// 생성된 위젯을 뷰포트에 출력한다.
			startUI->AddToViewport();

			// 게임을 일시정시 상태로 만든다.
			UGameplayStatics::SetGamePaused(GetWorld(), true);

			// 마우스 커서 나타내기
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
			// 생성된 위젯을 뷰포트에 출력한다.
			startUI->AddToViewport();

			// 게임을 일시정시 상태로 만든다.
			UGameplayStatics::SetGamePaused(GetWorld(), true);

			// 마우스 커서 나타내기
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
		}

	}

}



