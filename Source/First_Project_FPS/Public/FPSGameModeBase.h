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


	UPROPERTY(EditAnywhere)
		TSubclassOf<class AEnemyBoss> bossActor;


	UPROPERTY(EditAnywhere)
		int32 bossScore = 10;

	UPROPERTY(EditAnywhere)
		FVector startLocation = {0,200,400};


protected:
	virtual void BeginPlay() override;


private:
	// 현재 점수 저장용 변수
	int32 currentScore = 0;

	// 현재 뷰 포트에 로드된 위젯 저장용 변수
	class UMainWidget* mainUI;

	class UStartWidget* startUI;



	void PrintScore();

	void SpawnBoss();

	bool isSpawnBoss = false;




};
