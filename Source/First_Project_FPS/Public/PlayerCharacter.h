// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class FIRST_PROJECT_FPS_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	// SpringArm 선언
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* springArmComp;


	// Camera component 선언
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* tpsCamComp;

	// 좌우 회전 입력처리
	void Turn(float value);

	// 상하 회전 입력 처리
	void LookUp(float value);

	// 이동속도
	UPROPERTY(EditAnywhere, Category = "PlayerSetting")
		float walkSpeed = 600;

	// 이동방향
	FVector direction;

	// 좌우 이동 입력 이벤트 처리 함수
	void InputHorizontal(float value);

	// 상하 이동 입력 이벤트 처리 함수
	void InputVertical(float value);

	// 점프 입력 이벤트 처리 함수
	void InputJump();


	// 플레이어 이동 처리 함수
	void Move();
};
