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

	// 1. ������Ʈ ����
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));

	// 1-2. ��Ʈ ������Ʈ�� ����
	SetRootComponent(boxComp);

	// 1-3. box ������Ʈ size ����
	boxComp->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));

	// 2. ����ƽ �޽� ������Ʈ ����
	meshComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));

	// 2-1. boxComp ���� ������Ʈ�� ����
	meshComp->SetupAttachment(boxComp);

	// 2-2. ����ƽ �޽� ��ġ ����
	meshComp->SetRelativeLocation(FVector(0, 0, -50));

}

// Called when the game starts or when spawned
void AEnemySimple::BeginPlay()
{
	Super::BeginPlay();

	// 1~ 100 ������ ������ ���� ���� ��÷�Ѵ�.
	int32 drawResult = FMath::RandRange(1, 100);

	// ����, ��÷�� ���� ���� Ȯ�� �������� �۰ų� ���ٸ�..
	if (drawResult <= traceRate) {
		
		// ���� ������ APlayerPawn Ŭ������ �� ���͸� ��� �˻��Ѵ�.
		for (TActorIterator<APlayerCharacter> player(GetWorld()); player; ++player) {
			// ���� �˻��� ������ �̸��� "BP_PlayerPawn"�̶� ������ ���ԵǾ� �ִٸ�
			if (player->GetName().Contains(TEXT("BP_PlayerCharacter"))) {
				// �÷��̾� ������ ��ġ - �ڽ��� ��ġ
				dir = player->GetActorLocation() - GetActorLocation();
				dir.Normalize();
			}
		}
	}
	else // �׷��� �ʴٸ� ���� ���� ���͸� �����Ѵ�.
	{
		dir = GetActorForwardVector();
	}
	
}

// Called every frame
void AEnemySimple::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// BeginPlay()���� ������ �������� �̵��Ѵ�.
	FVector newLocation = GetActorLocation() + dir * moveSpeed * DeltaTime;
	SetActorLocation(newLocation);

}

