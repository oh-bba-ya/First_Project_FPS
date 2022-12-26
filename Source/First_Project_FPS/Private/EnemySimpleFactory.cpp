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

	// ����, ����� �ð��� ������ �ð��� �ʰ��ߴٸ�.
	if (currentTime > delayTime) {
		
		// ����� �ð��� 0�ʷ� �ʱ�ȭ�Ѵ�.
		currentTime = 0;

		// enemy ������ �Ҵ�� �������Ʈ�� �ڽ��� ��ġ�� �����Ѵ�.
		AEnemySimple* spawnActor = GetWorld()->SpawnActor<AEnemySimple>(enemy, GetActorLocation(), GetActorRotation());

	}
	else  // �׷��� �ʴٸ�
	{
		currentTime += DeltaTime;
	}

}

