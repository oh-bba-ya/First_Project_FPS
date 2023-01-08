// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"			// 헤더파일 추가
#include "Bullet.h"							// 총알 클래스 추가.
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





	// 1. 스켈레탈메시 데이터를 불러오고 싶다.


	//"/Script/Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"
	// /Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'"));
	if (TempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(TempMesh.Object);

		// 2.Mesh 컴포넌트의 위치와 회전값을 설정하고 싶다.
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, 0, 0));

		originMeshRot = GetMesh()->GetRelativeRotation();


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
		originGunMeshLoc = gunMeshComp->GetRelativeLocation();
		originGunMeshRot = gunMeshComp->GetRelativeRotation();
	}

	// 5. 스나이퍼건(레이저총) 컴포넌트 등록
	sniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGunComp"));
	
	// 5-1. 부모 컴포넌트를 Mesh 컴포넌트로 설정
	sniperGunComp->SetupAttachment(GetMesh());

	// 5-2. 스태틱메시 데이터 로드 (임시로 Pipe(파이프)로 함)
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Pipe.Shape_Pipe'"));

	// 5-3. 데이터 로드가 성공했다면
	if (TempSniperMesh.Succeeded()) {
		// 5-4. 스태틱 메시 데이터 할당
		sniperGunComp->SetStaticMesh(TempSniperMesh.Object);

		// 5-5. 위치 조정하기
		sniperGunComp->SetRelativeLocation(FVector(-22, 55, 120));

		// 5-6. 크기 조정하기
		sniperGunComp->SetRelativeScale3D(FVector(0.15f));

		originSniperMeshLoc = sniperGunComp->GetRelativeLocation();
		originSniperMeshRot = sniperGunComp->GetRelativeRotation();
	}


	// 초기 체력 설정
	hp = initialHp;

	// Arrow Component 생성
	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnArrow"));

	arrowComp->SetupAttachment(gunMeshComp);

	arrowComp->SetRelativeLocationAndRotation(FVector(0,60,0), FRotator(90.f,0,0));




}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	myGM = Cast<AFPSGameModeBase>(GetWorld()->GetAuthGameMode());

	// 기본으로 스나이퍼건을 사용하도록 설정.
	ChangeToSniperGun();


	// main 위젯 설정
	if (hpBarWidget != nullptr) {
		// mainWidget 블루프린트 파일을 메모리에 로드한다.
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


	// 좌우 입력 이벤트 처리 함수 바인딩
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &APlayerCharacter::InputHorizontal);


	// 위 방향 입력 이벤트 처리 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("DirectionUp"),IE_Pressed, this, &APlayerCharacter::InputDirectionUp);
	PlayerInputComponent->BindAction(TEXT("DirectionUp"), IE_Released, this, &APlayerCharacter::InputDirectionUp);


	// 점프 입력 이벤트 처리 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APlayerCharacter::InputJump);

	// 총알 발사 이벤트 처리 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &APlayerCharacter::InputFire);

	// 총 교체 이벤트 처리 함수 바인딩
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
	// 유탄총 사용시
	if (bUsingGrenadeGun && bombCount >0) {
		FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
		bombCount--;
		hpBarUI->bombCount->SetText(FText::AsNumber(bombCount));

	}
	else if(!bUsingGrenadeGun) // 스나이퍼건 사용시
	{

		// LineTrace의 시작 위치
		FVector startPos = arrowComp->GetComponentLocation();


		// LineTrace의 종료 위치
		FVector endPos = arrowComp->GetComponentLocation() + arrowComp->GetForwardVector() * 5000;

		// LineTrace의 충돌 정보를 담을 변수
		FHitResult hitInfo;

		// 충돌 옵션 설정 변수
		FCollisionQueryParams params;

		// 자기 자신(플레이어)는 충돌에서 제외
		params.AddIgnoredActor(this);

		// Channel 필터를 이용한 LineTrace 충돌 검충(충돌 정보, 시작 위치, 종료 위치, 검출 채널, 충돌 옵션)
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);

		// LineTrace가 부딪혔을 때
		if (bHit) {
			// 충돌 처리-> 총알 파편 효과 재생
			FTransform bulletTrans;

			// 부딪힌 위치 할당
			bulletTrans.SetLocation(hitInfo.ImpactPoint);

			// 총알 파편 효과 인스턴스 생성
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, bulletTrans);

			// 맞을 물체 컴포넌트 
			auto hitComp = hitInfo.GetComponent();

			// 1. 만약 컴포넌트에 물리가 적용되어 있다면
			if (hitComp && hitComp->IsSimulatingPhysics()) {
				// 2. 날려버릴 힘과 방향이 필요 , F = ma 뉴턴 운동법칙
				FVector force = -hitInfo.ImpactNormal * hitComp->GetMass() * 500000;

				// 3. 그 방향으로 날린다. 
				hitComp->AddForce(force);
			}
			else { // 만약 물리가 없다면
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
	// 유탄총 사용 중으로 체크
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




