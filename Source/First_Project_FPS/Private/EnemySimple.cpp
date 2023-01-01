// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySimple.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EngineUtils.h"
#include "PlayerCharacter.h"
#include "Math/Vector.h"

// Sets default values
AEnemySimple::AEnemySimple()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 1. 컴포넌트 생성
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));

	// 1-2. 루트 컴포넌트로 설정
	SetRootComponent(boxComp);

	// 1-3. box 컴포넌트 size 설정
	boxComp->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));

	// 2. 스태틱 메시 컴포넌트 생성
	meshComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));

	// 2-1. boxComp 하위 컴포넌트로 부착
	meshComp->SetupAttachment(boxComp);

	// 2-2. 스태틱 메쉬 위치 조정
	meshComp->SetRelativeLocation(FVector(0, 0, -50));


	// Collision Presets을 Enemy 프리셋으로 변경
	boxComp->SetCollisionProfileName(TEXT("EnemyBullet"));

}

// Called when the game starts or when spawned
void AEnemySimple::BeginPlay()
{
	Super::BeginPlay();
	
	// CollisionComponent의 충돌 오버랩 이벤트에 BulletOverlap 함수를 연결한다.
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemySimple::OnBulletOverlap);

	// Destoty()를 위한 타이머 설정 , 
	FTimerHandle deathTimer;

	// 다음 코드는 람다 함수를 사용
	GetWorld()->GetTimerManager().SetTimer(deathTimer,
		FTimerDelegate::CreateLambda([this]()->void {
			Destroy();
			})
		, 8.0f, false);

	SetDirection();
}

// Called every frame
void AEnemySimple::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// BeginPlay()에서 결정된 방향으로 이동한다.
	Fire(DeltaTime);

}


// Called every frame
void AEnemySimple::Fire(float deltaTime)
{
	FVector newLocation = GetActorLocation() + dir * moveSpeed * deltaTime;
	if (!isBossFire) {
		// BeginPlay()에서 결정된 방향으로 이동한다.
		SetActorLocation(newLocation);
	}
	else {
		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, newLocation.Z));
	}


}

// Called every frame
void AEnemySimple::SetDirection()
{

	// 월드 공간에 APlayerCharacter 클래스로 된 액터를 모두 검색한다.
	for (TActorIterator<APlayerCharacter> player(GetWorld()); player; ++player) {
		// 만일 검색된 액터의 이름에 "BP_PlayerPawn"이란 문구가 포함되어 있다면
		if (player->GetName().Contains(TEXT("BP_PlayerCharacter"))) {
			// 플레이어 액터의 위치 - 자신의 위치
			dir = player->GetActorLocation() - GetActorLocation();

			// 플레이어와의 거리
			FVector length = player->GetActorLocation() - GetActorLocation();

			dir.Normalize();
		}
	}

}


void AEnemySimple::OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 플레이어 캐스팅
	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(OtherActor);

	if (playerCharacter != nullptr) {
		playerCharacter->OnHitEvent();

	}

	Destroy();

}


