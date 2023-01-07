// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class FIRST_PROJECT_FPS_API AFPSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void AddScore(int32 point);

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMainWidget>mainWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UStartWidget>startWidget;

	class APlayerCharacter* player;

	UFUNCTION()
	void ShowStart();

	UFUNCTION()
	void ShowEnd();



protected:
	virtual void BeginPlay() override;


private:
	// ���� ���� ����� ����
	int32 currentScore = 0;

	// ���� �� ��Ʈ�� �ε�� ���� ����� ����
	class UMainWidget* mainUI;

	class UStartWidget* startUI;



	void PrintScore();


};
