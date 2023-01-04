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
	// �ð� �ٽ��帧
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	// ���콺 Ŀ�� OFF
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);

	this->RemoveFromViewport();
}


void UStartWidget::Quit()
{
	APlayerController* player = GetWorld()->GetFirstPlayerController();

	UKismetSystemLibrary::QuitGame(GetWorld(), player, EQuitPreference::Quit, true);


}
