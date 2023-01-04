// Fill out your copyright notice in the Description page of Project Settings.


#include "StartWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"

void UStartWidget::NativeConstruct() {
	btn_Start->OnClicked.AddDynamic(this, &UStartWidget::StartGame);
	btn_Quit->OnClicked.AddDynamic(this, &UStartWidget::Quit);
}

void UStartWidget::StartGame() {
	// 시간 다시흐름
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	// 마우스 커서 OFF
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);

	this->RemoveFromViewport();
}


void UStartWidget::Quit()
{
	APlayerController* player = GetWorld()->GetFirstPlayerController();

	UKismetSystemLibrary::QuitGame(GetWorld(), player, EQuitPreference::Quit, true);


}
