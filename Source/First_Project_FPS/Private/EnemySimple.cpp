// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySimple.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EngineUtils.h"
#include "PlayerCharacter.h"

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

}

// Called when the game starts or when spawned
void AEnemySimple::BeginPlay()
{
	Super::BeginPlay();

	// 1~ 100 사이의 임의의 정수 값을 추첨한다.
	int32 drawResult = FMath::RandRange(1, 100);

	// 만일, 추첨된 값이 추적 확률 변수보다 작거나 같다면..
	if (drawResult <= traceRate) {
		
		// 월드 공간에 APlayerPawn 클래스로 된 액터를 모두 검색한다.
		for (TActorIterator<APlayerCharacter> player(GetWorld()); player; ++player) {
			// 만일 검색된 액터의 이름에 "BP_PlayerPawn"이란 문구가 포함되어 있다면
			if (player->GetName().Contains(TEXT("BP_PlayerCharacter"))) {
				// 플레이어 액터의 위치 - 자신의 위치
				dir = player->GetActorLocation() - GetActorLocation();
				dir.Normalize();
			}
		}
	}
	else // 그렇지 않다면 정면 방향 벡터를 생성한다.
	{
		dir = GetActorForwardVector();
	}
	
}

// Called every frame
void AEnemySimple::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// BeginPlay()에서 결정된 방향으로 이동한다.
	FVector newLocation = GetActorLocation() + dir * moveSpeed * DeltaTime;
	SetActorLocation(newLocation);

}

