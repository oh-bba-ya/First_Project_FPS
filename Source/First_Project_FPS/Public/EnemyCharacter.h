// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class FIRST_PROJECT_FPS_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	UFUNCTION()
		void EnemyHitEvent(int32 d = 1);

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AEnemySimple> enemyFactory;

	class APlayerCharacter* target;

	UPROPERTY(EditAnywhere, Category = "Enemy Setting")
		TSubclassOf<class APlayerCharacter> characterBP;

private:
	UPROPERTY(EditAnywhere, Category = "Enemy Settings")
		int32 enemyHP = 5;

	float curTime = 0;
	float coolTime = 2.0f;

	UPROPERTY(EditAnywhere, Category = "EnemySpawn Settings")
		float setDistance;

	class APlayerCharacter* character;
};
