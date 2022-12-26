// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySimpleFactory.generated.h"

UCLASS()
class FIRST_PROJECT_FPS_API AEnemySimpleFactory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySimpleFactory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float delayTime = 2.0f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEnemySimple> enemy;

private:
	float currentTime = 0;


};
