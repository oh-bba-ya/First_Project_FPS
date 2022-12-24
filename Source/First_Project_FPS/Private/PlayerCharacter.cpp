// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"			// ������� �߰�
#include "Bullet.h"							// �Ѿ� Ŭ���� �߰�.
#include "Kismet/GameplayStatics.h"


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

	// 5. �������۰�(��������) ������Ʈ ���
	sniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGunComp"));
	
	// 5-1. �θ� ������Ʈ�� Mesh ������Ʈ�� ����
	sniperGunComp->SetupAttachment(GetMesh());

	// 5-2. ����ƽ�޽� ������ �ε� (�ӽ÷� Pipe(������)�� ��)
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Pipe.Shape_Pipe'"));

	// 5-3. ������ �ε尡 �����ߴٸ�
	if (TempSniperMesh.Succeeded()) {
		// 5-4. ����ƽ �޽� ������ �Ҵ�
		sniperGunComp->SetStaticMesh(TempSniperMesh.Object);

		// 5-5. ��ġ �����ϱ�
		sniperGunComp->SetRelativeLocation(FVector(-22, 55, 120));

		// 5-6. ũ�� �����ϱ�
		sniperGunComp->SetRelativeScale3D(FVector(0.15f));
	}

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// �⺻���� �������۰��� ����ϵ��� ����.
	ChangeToSniperGun();
	
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

	// �� ��ü �̺�Ʈ ó�� �Լ� ���ε�
	PlayerInputComponent->BindAction(TEXT("GrenadeGun"), IE_Pressed, this, &APlayerCharacter::ChangeToGrenadeGun);
	PlayerInputComponent->BindAction(TEXT("SniperGun"), IE_Pressed, this, &APlayerCharacter::ChangeToSniperGun);

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
	// ��ź�� ����
	if (bUsingGrenadeGun) {
		FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);

	}
	else // �������۰� ����
	{

		// LineTrace�� ���� ��ġ
		FVector startPos = tpsCamComp->GetComponentLocation();


		// LineTrace�� ���� ��ġ
		FVector endPos = tpsCamComp->GetComponentLocation() + tpsCamComp->GetForwardVector() * 5000;

		// LineTrace�� �浹 ������ ���� ����
		FHitResult hitInfo;

		// �浹 �ɼ� ���� ����
		FCollisionQueryParams params;

		// �ڱ� �ڽ�(�÷��̾�)�� �浹���� ����
		params.AddIgnoredActor(this);

		// Channel ���͸� �̿��� LineTrace �浹 ����(�浹 ����, ���� ��ġ, ���� ��ġ, ���� ä��, �浹 �ɼ�)
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);

		// LineTrace�� �ε����� ��
		if (bHit) {
			// �浹 ó��-> �Ѿ� ���� ȿ�� ���
			FTransform bulletTrans;

			// �ε��� ��ġ �Ҵ�
			bulletTrans.SetLocation(hitInfo.ImpactPoint);

			// �Ѿ� ���� ȿ�� �ν��Ͻ� ����
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, bulletTrans);

			// ���� ��ü ������Ʈ 
			auto hitComp = hitInfo.GetComponent();

			// 1. ���� ������Ʈ�� ������ ����Ǿ� �ִٸ�
			if (hitComp && hitComp->IsSimulatingPhysics()) {
				// 2. �������� ���� ������ �ʿ� , F = ma ���� ���Ģ
				FVector force = -hitInfo.ImpactNormal * hitComp->GetMass() * 500000;

				// 3. �� �������� ������. 
				hitComp->AddForce(force);
			}
		}

	}

}

void APlayerCharacter::ChangeToGrenadeGun()
{
	// ��ź�� ��� ������ üũ
	bUsingGrenadeGun = true;
	sniperGunComp->SetVisibility(false);
	gunMeshComp->SetVisibility(true);
}

void APlayerCharacter::ChangeToSniperGun()
{
	bUsingGrenadeGun = false;
	sniperGunComp->SetVisibility(true);
	gunMeshComp->SetVisibility(false);
}


