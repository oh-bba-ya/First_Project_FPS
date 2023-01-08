// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"			// ������� �߰�
#include "Bullet.h"							// �Ѿ� Ŭ���� �߰�.
#include "Kismet/GameplayStatics.h"
#include "HpBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/ArrowComponent.h"
#include "EnemyBoss.h"
#include "EnemyCharacter.h"
#include "FPSGameModeBase.h"
#include "NiagaraFunctionLibrary.h"




// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;





	// 1. ���̷�Ż�޽� �����͸� �ҷ����� �ʹ�.


	//"/Script/Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"
	// /Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'"));
	if (TempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(TempMesh.Object);

		// 2.Mesh ������Ʈ�� ��ġ�� ȸ������ �����ϰ� �ʹ�.
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, 0, 0));

		originMeshRot = GetMesh()->GetRelativeRotation();


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
		originGunMeshLoc = gunMeshComp->GetRelativeLocation();
		originGunMeshRot = gunMeshComp->GetRelativeRotation();
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

		originSniperMeshLoc = sniperGunComp->GetRelativeLocation();
		originSniperMeshRot = sniperGunComp->GetRelativeRotation();
	}


	// �ʱ� ü�� ����
	hp = initialHp;

	// Arrow Component ����
	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnArrow"));

	arrowComp->SetupAttachment(gunMeshComp);

	arrowComp->SetRelativeLocationAndRotation(FVector(0,60,0), FRotator(90.f,0,0));




}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	myGM = Cast<AFPSGameModeBase>(GetWorld()->GetAuthGameMode());

	// �⺻���� �������۰��� ����ϵ��� ����.
	ChangeToSniperGun();


	// main ���� ����
	if (hpBarWidget != nullptr) {
		// mainWidget �������Ʈ ������ �޸𸮿� �ε��Ѵ�.
		hpBarUI = CreateWidget<UHpBar>(GetWorld(), hpBarWidget);
		if (hpBarUI != nullptr) {
			hpBarUI->AddToViewport();
			hpBarUI->hpBar->SetPercent(GetCurrentHealth() / GetMaxHealth() * 100);
			hpBarUI->bombCount->SetText(FText::AsNumber(bombCount));
			hpBarUI->machineGunCount->SetText(FText::AsNumber(machineGunCount));
		}

	}


	
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


	// �¿� �Է� �̺�Ʈ ó�� �Լ� ���ε�
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &APlayerCharacter::InputHorizontal);


	// �� ���� �Է� �̺�Ʈ ó�� �Լ� ���ε�
	PlayerInputComponent->BindAction(TEXT("DirectionUp"),IE_Pressed, this, &APlayerCharacter::InputDirectionUp);
	PlayerInputComponent->BindAction(TEXT("DirectionUp"), IE_Released, this, &APlayerCharacter::InputDirectionUp);


	// ���� �Է� �̺�Ʈ ó�� �Լ� ���ε�
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APlayerCharacter::InputJump);

	// �Ѿ� �߻� �̺�Ʈ ó�� �Լ� ���ε�
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &APlayerCharacter::InputFire);

	// �� ��ü �̺�Ʈ ó�� �Լ� ���ε�
	PlayerInputComponent->BindAction(TEXT("GrenadeGun"), IE_Pressed, this, &APlayerCharacter::ChangeToGrenadeGun);
	PlayerInputComponent->BindAction(TEXT("SniperGun"), IE_Pressed, this, &APlayerCharacter::ChangeToSniperGun);

}



void APlayerCharacter::InputHorizontal(float value)
{
	direction.Y = value;
	if (value < 0) {
		gunMeshComp->SetRelativeLocationAndRotation(originGunMeshLoc, originGunMeshRot);
		GetMesh()->SetRelativeRotation(FRotator(0,180.f,0));

	}
	else if(value > 0) {
		gunMeshComp->SetRelativeLocationAndRotation(originGunMeshLoc, originGunMeshRot);
		GetMesh()->SetRelativeRotation(originMeshRot);
	}
	
}


void APlayerCharacter::InputJump()
{
	Jump();
}

void APlayerCharacter::InputDirectionUp()
{
	FRotator rot = { 0,0,0 };
	FVector loc = { 0,0,0 };
	

	isUp = !isUp;
	if(isUp){
		loc = { 0,0,240.f };
		rot = { 0,0,-90.f };
		gunMeshComp->SetRelativeLocationAndRotation(loc, rot);
		sniperGunComp->SetRelativeLocationAndRotation(loc, rot);
	}
	else {
		gunMeshComp->SetRelativeLocationAndRotation(originGunMeshLoc, originGunMeshRot);
		sniperGunComp->SetRelativeLocationAndRotation(originSniperMeshLoc, originSniperMeshRot);
	}
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
	if (bUsingGrenadeGun && bombCount >0) {
		FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
		bombCount--;
		hpBarUI->bombCount->SetText(FText::AsNumber(bombCount));

	}
	else if(!bUsingGrenadeGun) // �������۰� ����
	{

		// LineTrace�� ���� ��ġ
		FVector startPos = arrowComp->GetComponentLocation();


		// LineTrace�� ���� ��ġ
		FVector endPos = arrowComp->GetComponentLocation() + arrowComp->GetForwardVector() * 5000;

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
			else { // ���� ������ ���ٸ�
				if (machineGunCount > 0) {
					machineGunCount -= 1;
					hpBarUI->machineGunCount->SetText(FText::AsNumber(machineGunCount));
					/*
					if (NS_Laser != nullptr) {
						UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_Laser,startPos, arrowComp->GetRelativeRotation());
					}
					*/

				}
				if (hitInfo.GetActor()->GetName().Contains(TEXT("BP_EnemyBoss"))) {
					AEnemyBoss* boss = Cast<AEnemyBoss>(hitInfo.GetActor());
					boss->OnHitEvent();

				}
				else if (hitInfo.GetActor()->GetName().Contains(TEXT("BP_EnemySimpleFactory"))) {
					hitInfo.GetActor()->Destroy();
				}
				else if (hitInfo.GetActor()->GetName().Contains(TEXT("BP_EnemyCharacter"))) {
					AEnemyCharacter* enemyCh = Cast<AEnemyCharacter>(hitInfo.GetActor());
					enemyCh->EnemyHitEvent(1);
				}

					


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


void APlayerCharacter::OnHitEvent()
{
	
	hp-= 10;
	hpBarUI->hpBar->SetPercent(GetCurrentHealth() / GetMaxHealth());
	if (hp <= 0) {
		hp = 0;
		myGM->ShowStart();
	}
}

float APlayerCharacter::GetMaxHealth()
{
	return initialHp;
}

float APlayerCharacter::GetCurrentHealth()
{
	return hp;
}

void APlayerCharacter::SetHealth(float health)
{
	hp = health;
}

void APlayerCharacter::AddBomb(int count)
{
	bombCount += count;
	hpBarUI->bombCount->SetText(FText::AsNumber(bombCount));
}

void APlayerCharacter::AddMachineGun(int count)
{
	machineGunCount += count;
	hpBarUI->machineGunCount->SetText(FText::AsNumber(machineGunCount));
}




