// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"			// 헤더파일 추가
#include "Bullet.h"							// 총알 클래스 추가.

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 1. 스켈레탈메시 데이터를 불러오고 싶다.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));
	if (TempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(TempMesh.Object);

		// 2.Mesh 컴포넌트의 위치와 회전값을 설정하고 싶다.
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	// 3.TPS 카메라 부착
	// 3-1 SpringArm 컴포넌트 부착
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90));
	springArmComp->TargetArmLength = 400;

	// 마우스를 이용한 회전처리
	springArmComp->bUsePawnControlRotation = true;

	// 3-2. CameraComponent 부착
	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);

	// 마우스를 이용한 회전처리
	tpsCamComp->bUsePawnControlRotation = false;

	// 마우스를 이용한 회전처리
	bUseControllerRotationYaw = true;


	// 4. 총 스켈레탈메시 컴포넌트 등록
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));

	// 4-1. 부모 컴포넌트를 Mesh컴포넌트로 설정
	gunMeshComp->SetupAttachment(GetMesh());

	// 4-2. 스켈레탈 메시 데이터 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/MyResources/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

	// 4-3 데이터로드가 성공했다면
	if (TempGunMesh.Succeeded()) {
		// 4-4. 스켈레탈메시 데이터 할당
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		// 4-5. 위치 조정하기
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

	// Turn , LookUp 바인딩
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APlayerCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayerCharacter::LookUp);

	// 좌우 입력 이벤트 처리 함수 바인딩
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &APlayerCharacter::InputHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &APlayerCharacter::InputVertical);

	// 점프 입력 이벤트 처리 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APlayerCharacter::InputJump);

	// 총알 발사 이벤트 처리 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &APlayerCharacter::InputFire);

}


// 좌우 회전 입력 정의
void APlayerCharacter::Turn(float value) {
	AddControllerYawInput(value);
}

// 상하 회전 입력 정의
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
	// 마우스 즉, 캐릭터가 바라보는 방향으로 방향 설정
	direction = FTransform(GetControlRotation()).TransformVector(direction);

	// 플레이어 이동처리 (등속운동) , P(결과위치) = P0(현재위치) + V(속도) *t(시간)
	/*
	FVector P0 = GetActorLocation();
	FVector vt = direction * walkSpeed * DeltaTime;
	FVector P = P0 + vt;
	SetActorLocation(P);
	*/

	AddMovementInput(direction); // 위의 과정을 제공되는 함수로 처리할 수 있다..

	direction = FVector::ZeroVector;
}

void APlayerCharacter::InputFire()
{
	FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
}


