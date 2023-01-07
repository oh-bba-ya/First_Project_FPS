// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBoss.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerCharacter.h"
#include "EngineUtils.h"
#include "Math/UnrealMathUtility.h"
#include "EnemySimple.h"
#include "FPSGameModeBase.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AEnemyBoss::AEnemyBoss()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(boxComp);

	// 1-3. box 컴포넌트 size 설정
	boxComp->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	meshComp->SetupAttachment(boxComp);

	// 2-2. 스태틱 메쉬 위치 조정
	meshComp->SetRelativeLocation(FVector(0, 0, -50));




}

// Called when the game starts or when spawned
void AEnemyBoss::BeginPlay()
{
	Super::BeginPlay();

	SetDirection();

}

// Called every frame
void AEnemyBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	Move(DeltaTime);

}

void AEnemyBoss::SetDirection()
{

	for (TActorIterator<APlayerCharacter> player(GetWorld()); player; ++player) {
		if (player->GetName().Contains(TEXT("BP_PlayerCharacter"))) {

			playerPos = player->GetActorLocation();

			// 플레이어 액터의 위치 - 자신의 위치
			direction = playerPos - GetActorLocation();

			direction.Normalize();
		}
	}
	

	
}

void AEnemyBoss::Move(float deltaTime)
{

	FTimerHandle moveTime;

	FTimerHandle fireTime;

	FVector newLocation = GetActorLocation() + direction * moveSpeed * deltaTime;

	float x = FMath::Square(FMath::Abs(playerPos.X - GetActorLocation().X));
	float y = FMath::Square(FMath::Abs(playerPos.Y - GetActorLocation().Y));
	float dist = FMath::Sqrt(x + y);

	// 플레이어 위에 도착 Fire, 3초 뒤 다시 방향 재설정
	if ( dist <3) {

		curTime += deltaTime;

		if (curTime >= spawnTime && curCount <= maxCount) {
			curTime = 0;
			curCount++;
			if (curCount > maxCount) {
				curCount = 0;
			}
			Fire();
		}



		GetWorld()->GetTimerManager().SetTimer(moveTime,
			FTimerDelegate::CreateLambda([this]()->void {
				SetDirection();
				})
			, moveInterval, false);
	}
	else {
		SetActorLocation(FVector(newLocation.X, newLocation.Y, GetActorLocation().Z));
	}

	

}

void AEnemyBoss::Fire()
{

	AEnemySimple* enemyBulletActor = GetWorld()->SpawnActor<AEnemySimple>(enemyBullet, GetActorLocation(), GetActorRotation());
}

void AEnemyBoss::OnHitEvent(int32 d)
{
	if (bossHP > 0) {
		bossHP-=d;
		if (bossHP == 0) {
			Destroy();
			myGameMode = Cast<AFPSGameModeBase>(GetWorld()->GetAuthGameMode());
			myGameMode->ShowEnd();
		}
	}
	else {
		Destroy();
	}
}



