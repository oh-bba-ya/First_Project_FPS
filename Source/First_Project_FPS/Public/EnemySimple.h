// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySimple.generated.h"

UCLASS()
class FIRST_PROJECT_FPS_API AEnemySimple : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySimple();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* meshComp;

	// �ش��ϴ� ���� ���� ���� (0 ~ traceRate ������� �÷��̾� ����)
	UPROPERTY(EditAnywhere)
		int32 traceRate = 50;

	// �̵� �ӵ� ����
	UPROPERTY(EditAnywhere)
		float moveSpeed = 800;

	void SetDirection();

	void Fire(float deltaTime);

	UPROPERTY(EditAnywhere)
		bool isBossFire = false;





	UFUNCTION()
	void OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


private:
	// Enemy���� �÷��̾� ����
	FVector dir;

	

};
