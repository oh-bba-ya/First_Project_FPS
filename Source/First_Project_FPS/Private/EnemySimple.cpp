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


	// Collision Presets�� Enemy ���������� ����
	boxComp->SetCollisionProfileName(TEXT("EnemyBullet"));

}

// Called when the game starts or when spawned
void AEnemySimple::BeginPlay()
{
	Super::BeginPlay();
	
	// CollisionComponent�� �浹 ������ �̺�Ʈ�� BulletOverlap �Լ��� �����Ѵ�.
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemySimple::OnBulletOverlap);

	// Destoty()�� ���� Ÿ�̸� ���� , 
	FTimerHandle deathTimer;

	// ���� �ڵ�� ���� �Լ��� ���
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

	// BeginPlay()���� ������ �������� �̵��Ѵ�.
	Fire(DeltaTime);

}


// Called every frame
void AEnemySimple::Fire(float deltaTime)
{
	FVector newLocation = GetActorLocation() + dir * moveSpeed * deltaTime;
	if (!isBossFire) {
		// BeginPlay()���� ������ �������� �̵��Ѵ�.
		SetActorLocation(newLocation);
	}
	else {
		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, newLocation.Z));
	}


}

// Called every frame
void AEnemySimple::SetDirection()
{

	// ���� ������ APlayerCharacter Ŭ������ �� ���͸� ��� �˻��Ѵ�.
	for (TActorIterator<APlayerCharacter> player(GetWorld()); player; ++player) {
		// ���� �˻��� ������ �̸��� "BP_PlayerPawn"�̶� ������ ���ԵǾ� �ִٸ�
		if (player->GetName().Contains(TEXT("BP_PlayerCharacter"))) {
			// �÷��̾� ������ ��ġ - �ڽ��� ��ġ
			dir = player->GetActorLocation() - GetActorLocation();

			// �÷��̾���� �Ÿ�
			FVector length = player->GetActorLocation() - GetActorLocation();

			dir.Normalize();
		}
	}

}


void AEnemySimple::OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// �÷��̾� ĳ����
	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(OtherActor);

	if (playerCharacter != nullptr) {
		playerCharacter->OnHitEvent();

	}

	Destroy();

}


