// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySimpleFactory.h"
#include "EnemySimple.h"

// Sets default values
AEnemySimpleFactory::AEnemySimpleFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

