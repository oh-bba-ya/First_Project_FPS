// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBoss.generated.h"

UCLASS()
class FIRST_PROJECT_FPS_API AEnemyBoss : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyBoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Boss Settings")
		class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere, Category = "Boss Settings")
		class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, Category = "Boss Settings")
		TSubclassOf<class AEnemySimple> enemyBullet;



	void SetDirection();

	void Move(float deltaTime);

	void Fire();

	UFUNCTION()
	void OnHitEvent(int32 d = 1);


private:
	UPROPERTY(EditAnywhere, Category = "Boss Settings")
		int32 bossHP = 10;

	UPROPERTY(EditAnywhere, Category = "Boss Settings")
	float moveSpeed = 100;

	UPROPERTY(EditAnywhere, Category = "Boss Settings")
		float moveInterval = 3.5f;

	FVector direction;

	FVector playerPos;

	int maxCount = 3;

	int curCount = 0;

	UPROPERTY(EditAnywhere, Category = "Boss Settings")
		float spawnTime = 1.f;

	float curTime = 0;

	class AFPSGameModeBase* myGameMode;





};
