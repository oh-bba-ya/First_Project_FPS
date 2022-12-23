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
	// SpringArm ����
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* springArmComp;


	// Camera component ����
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* tpsCamComp;

	// �¿� ȸ�� �Է�ó��
	void Turn(float value);

	// ���� ȸ�� �Է� ó��
	void LookUp(float value);

	// �̵��ӵ�
	UPROPERTY(EditAnywhere, Category = "PlayerSetting")
		float walkSpeed = 600;

	// �̵�����
	FVector direction;

	// �¿� �̵� �Է� �̺�Ʈ ó�� �Լ�
	void InputHorizontal(float value);

	// ���� �̵� �Է� �̺�Ʈ ó�� �Լ�
	void InputVertical(float value);

	// ���� �Է� �̺�Ʈ ó�� �Լ�
	void InputJump();


	// �÷��̾� �̵� ó�� �Լ�
	void Move();
};
