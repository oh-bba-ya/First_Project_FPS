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

	// 총 스켈레탈 메시
	UPROPERTY(VisibleAnywhere, Category = GunMesh)
		class USkeletalMeshComponent* gunMeshComp;

	// 총알 공장
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
		TSubclassOf<class ABullet> bulletFactory;

	// 총알 발사 처리 함수
	void InputFire();

	// 스나이퍼건(레이저총) 스태틱메시 추가
	UPROPERTY(VisibleAnywhere, Category = GunMesh)
		class UStaticMeshComponent* sniperGunComp;

	// 유탄총 사용 중인지 여부
	bool bUsingGrenadeGun = true;

	// 유탄총으로 변경
	void ChangeToGrenadeGun();

	// 스나이퍼건으로 변경
	void ChangeToSniperGun();

	// 총알 파편 효과 공장
	UPROPERTY(EditAnywhere, Category = BulletEffect)
		class UParticleSystem* bulletEffectFactory;

	// 피격 당했을 때 처리
	void OnHitEvent();

	UFUNCTION()
	float GetMaxHealth();

	UFUNCTION()
	float GetCurrentHealth();

	UFUNCTION()
	void SetHealth(float health);

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UHpBar> hpBarWidget;


	UPROPERTY(meta = (BindWidget))
		class UProgressBar* PB_hpBar;

	void AddBullet(int count);

private:
	// 현재 체력
	float hp;

	// 초기 HP 값
	UPROPERTY(EditAnywhere, Category = Health)
	float initialHp = 100;

	// 현재 뷰 포트에 로드된 위젯 저장용 변수
	class UHpBar* hpBarUI;

	int ammo = 0;


	
		
};