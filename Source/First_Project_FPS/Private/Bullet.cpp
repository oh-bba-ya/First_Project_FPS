// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"		// 헤더파일 추가
#include "EnemySimple.h"
#include "EnemyCharacter.h"
#include "FPSGameModeBase.h"
#include "EnemySimpleFactory.h"
#include "EnemyBoss.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 1. 충돌체 등록하기
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));

	// 2. 충돌프로파일 설정
	collisionComp->SetCollisionProfileName(TEXT("BlockAll"));

	// 3. 충돌체 크기 설정
	collisionComp->SetSphereRadius(13);

	// 4.루트로 등록
	RootComponent = collisionComp;

	// 5. 외관 컴포넌트 등록하기
	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));

	// 6. 부모 컴포넌트 지정 (루트 컴포넌트)
	bodyMeshComp->SetupAttachment(collisionComp);		// bodyMeshComp->SetupAttachment(RootComponent)

	// 7. 충돌 비활성화
	bodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 8. 외관 크기 설정
	bodyMeshComp->SetRelativeScale3D(FVector(0.25f));

	// 발사체 컴포넌트
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));

	// movement 컴포넌트가 갱신시킬 컴포넌트 지정
	movementComp->SetUpdatedComponent(collisionComp);

	// 초기속도
	movementComp->InitialSpeed = 1000;

	// 최대속도
	movementComp->MaxSpeed = 1000;

	// 반동 여부
	movementComp->bShouldBounce = true;

	// 반동 값
	movementComp->Bounciness = 0.3f;

	// 중력
	movementComp->ProjectileGravityScale = 5.f;

	// 회전
	movementComp->bRotationFollowsVelocity = true;

	// 생명 시간 주기 (단순한 제거 방법 2가지 중 첫번째 )
	//InitialLifeSpan = 2.0f;


	// Collision presets을 PlayerBullet으로 설정
	collisionComp->SetCollisionProfileName(TEXT("PlayerBullet"));




}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	// CollisionComponent의 충돌 오버랩 이벤트에 BulletOverlap 함수를 연결한다.
	collisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnBulletOverlap);

	// Destoty()를 위한 타이머 설정 , 
	FTimerHandle deathTimer;
	// 다음 코드는 일반 함수(Die) 를 만들어서 이용
	//GetWorld()->GetTimerManager().SetTimer(deathTimer, this, &ABullet::Die, 2.0f, false);

	// 다음 코드는 람다 함수를 사용
	GetWorld()->GetTimerManager().SetTimer(deathTimer, 
		FTimerDelegate::CreateLambda([this]()->void {
			Destroy();
		})
		, 5.0f, false);
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::Die()
{
	Destroy();
}

void ABullet::OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (NS_Explosion != nullptr) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_Explosion, GetActorLocation());
	}

	if (SB_Explosion != nullptr) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SB_Explosion, GetActorLocation());
	}


	// 현재 게임 모드를 가져온다.
	AGameModeBase* currentMode = GetWorld()->GetAuthGameMode();

	// AFPSGameModeBase로 변환한다.
	AFPSGameModeBase* currentGameModeBase = Cast<AFPSGameModeBase>(currentMode);


	// EnemyFactory 캐스팅
	AEnemySimpleFactory* enemyFactory = Cast<AEnemySimpleFactory>(OtherActor);

	if (enemyFactory != nullptr) {
		enemyFactory->Destroy();

		if (currentGameModeBase != nullptr) {
			currentGameModeBase->AddScore(1);
		}
	}

	// EnemyCharacter 캐스팅
	AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>(OtherActor);

	if (enemyCharacter != nullptr) {
		enemyCharacter->EnemyHitEvent(damage);

	}

	// EnemyBoss 캐스팅
	AEnemyBoss* enemyBoss = Cast<AEnemyBoss>(OtherActor);

	if (enemyBoss != nullptr) {
		enemyBoss->OnHitEvent(damage);

	}
	
	Destroy();

}

