// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"			// ������� �߰�
#include "Bullet.h"							// �Ѿ� Ŭ���� �߰�.

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 1. ���̷�Ż�޽� �����͸� �ҷ����� �ʹ�.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));
	if (TempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(TempMesh.Object);

		// 2.Mesh ������Ʈ�� ��ġ�� ȸ������ �����ϰ� �ʹ�.
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	// 3.TPS ī�޶� ����
	// 3-1 SpringArm ������Ʈ ����
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90));
	springArmComp->TargetArmLength = 400;

	// ���콺�� �̿��� ȸ��ó��
	springArmComp->bUsePawnControlRotation = true;

	// 3-2. CameraComponent ����
	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);

	// ���콺�� �̿��� ȸ��ó��
	tpsCamComp->bUsePawnControlRotation = false;

	// ���콺�� �̿��� ȸ��ó��
	bUseControllerRotationYaw = true;


	// 4. �� ���̷�Ż�޽� ������Ʈ ���
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));

	// 4-1. �θ� ������Ʈ�� Mesh������Ʈ�� ����
	gunMeshComp->SetupAttachment(GetMesh());

	// 4-2. ���̷�Ż �޽� ������ �ε�
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/MyResources/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

	// 4-3 �����ͷε尡 �����ߴٸ�
	if (TempGunMesh.Succeeded()) {
		// 4-4. ���̷�Ż�޽� ������ �Ҵ�
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		// 4-5. ��ġ �����ϱ�
		gunMeshComp->SetRelativeLocation(FVector(-14, 52, 120));
	}

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move();

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Turn , LookUp ���ε�
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APlayerCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayerCharacter::LookUp);

	// �¿� �Է� �̺�Ʈ ó�� �Լ� ���ε�
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &APlayerCharacter::InputHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &APlayerCharacter::InputVertical);

	// ���� �Է� �̺�Ʈ ó�� �Լ� ���ε�
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APlayerCharacter::InputJump);

	// �Ѿ� �߻� �̺�Ʈ ó�� �Լ� ���ε�
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &APlayerCharacter::InputFire);

}


// �¿� ȸ�� �Է� ����
void APlayerCharacter::Turn(float value) {
	AddControllerYawInput(value);
}

// ���� ȸ�� �Է� ����
void APlayerCharacter::LookUp(float value) {
	AddControllerPitchInput(value);
}

void APlayerCharacter::InputHorizontal(float value)
{
	direction.Y = value;
}

void APlayerCharacter::InputVertical(float value)
{
	direction.X = value;
}

void APlayerCharacter::InputJump()
{
	Jump();
}

void APlayerCharacter::Move()
{
	// ���콺 ��, ĳ���Ͱ� �ٶ󺸴� �������� ���� ����
	direction = FTransform(GetControlRotation()).TransformVector(direction);

	// �÷��̾� �̵�ó�� (��ӿ) , P(�����ġ) = P0(������ġ) + V(�ӵ�) *t(�ð�)
	/*
	FVector P0 = GetActorLocation();
	FVector vt = direction * walkSpeed * DeltaTime;
	FVector P = P0 + vt;
	SetActorLocation(P);
	*/

	AddMovementInput(direction); // ���� ������ �����Ǵ� �Լ��� ó���� �� �ִ�..

	direction = FVector::ZeroVector;
}

void APlayerCharacter::InputFire()
{
	FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
}


