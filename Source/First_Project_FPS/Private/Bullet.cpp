// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"		// ������� �߰�
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

	// 1. �浹ü ����ϱ�
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));

	// 2. �浹�������� ����
	collisionComp->SetCollisionProfileName(TEXT("BlockAll"));

	// 3. �浹ü ũ�� ����
	collisionComp->SetSphereRadius(13);

	// 4.��Ʈ�� ���
	RootComponent = collisionComp;

	// 5. �ܰ� ������Ʈ ����ϱ�
	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));

	// 6. �θ� ������Ʈ ���� (��Ʈ ������Ʈ)
	bodyMeshComp->SetupAttachment(collisionComp);		// bodyMeshComp->SetupAttachment(RootComponent)

	// 7. �浹 ��Ȱ��ȭ
	bodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 8. �ܰ� ũ�� ����
	bodyMeshComp->SetRelativeScale3D(FVector(0.25f));

	// �߻�ü ������Ʈ
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));

	// movement ������Ʈ�� ���Ž�ų ������Ʈ ����
	movementComp->SetUpdatedComponent(collisionComp);

	// �ʱ�ӵ�
	movementComp->InitialSpeed = 1000;

	// �ִ�ӵ�
	movementComp->MaxSpeed = 1000;

	// �ݵ� ����
	movementComp->bShouldBounce = true;

	// �ݵ� ��
	movementComp->Bounciness = 0.3f;

	// �߷�
	movementComp->ProjectileGravityScale = 5.f;

	// ȸ��
	movementComp->bRotationFollowsVelocity = true;

	// ���� �ð� �ֱ� (�ܼ��� ���� ��� 2���� �� ù��° )
	//InitialLifeSpan = 2.0f;


	// Collision presets�� PlayerBullet���� ����
	collisionComp->SetCollisionProfileName(TEXT("PlayerBullet"));




}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	// CollisionComponent�� �浹 ������ �̺�Ʈ�� BulletOverlap �Լ��� �����Ѵ�.
	collisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnBulletOverlap);

	// Destoty()�� ���� Ÿ�̸� ���� , 
	FTimerHandle deathTimer;
	// ���� �ڵ�� �Ϲ� �Լ�(Die) �� ���� �̿�
	//GetWorld()->GetTimerManager().SetTimer(deathTimer, this, &ABullet::Die, 2.0f, false);

	// ���� �ڵ�� ���� �Լ��� ���
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


	// ���� ���� ��带 �����´�.
	AGameModeBase* currentMode = GetWorld()->GetAuthGameMode();

	// AFPSGameModeBase�� ��ȯ�Ѵ�.
	AFPSGameModeBase* currentGameModeBase = Cast<AFPSGameModeBase>(currentMode);


	// EnemyFactory ĳ����
	AEnemySimpleFactory* enemyFactory = Cast<AEnemySimpleFactory>(OtherActor);

	if (enemyFactory != nullptr) {
		enemyFactory->Destroy();

		if (currentGameModeBase != nullptr) {
			currentGameModeBase->AddScore(1);
		}
	}

	// EnemyCharacter ĳ����
	AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>(OtherActor);

	if (enemyCharacter != nullptr) {
		enemyCharacter->EnemyHitEvent(damage);

	}

	// EnemyBoss ĳ����
	AEnemyBoss* enemyBoss = Cast<AEnemyBoss>(OtherActor);

	if (enemyBoss != nullptr) {
		enemyBoss->OnHitEvent(damage);

	}
	
	Destroy();

}

