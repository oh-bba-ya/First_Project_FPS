// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySimpleFactory.h"
#include "EnemySimple.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AEnemySimpleFactory::AEnemySimpleFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));

	boxComp->SetCollisionProfileName(TEXT("EnemySimple"));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(boxComp);
	meshComp->SetRelativeLocation(FVector(0, 0, -50.0f));

	

}

// Called when the game starts or when spawned
void AEnemySimpleFactory::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemySimpleFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 만일, 경과된 시간이 생성할 시간을 초과했다면.
	if (currentTime > delayTime) {
		
		// 경과된 시간을 0초로 초기화한다.
		currentTime = 0;

		// enemy 변수에 할당된 블루프린트를 자신의 위치에 생성한다.
		AEnemySimple* spawnActor = GetWorld()->SpawnActor<AEnemySimple>(enemy, GetActorLocation(), GetActorRotation());

	}
	else  // 그렇지 않다면
	{
		currentTime += DeltaTime;
	}

}

