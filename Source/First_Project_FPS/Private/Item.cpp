// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerCharacter.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>("Box Component");
	SetRootComponent(boxComp);

	boxComp->SetBoxExtent(FVector(50, 50, 50));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	meshComp->SetupAttachment(RootComponent);


	meshComp->SetCollisionProfileName(TEXT("EnemyBullet"));

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AItem::PickUpOverlap);

	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AutoRotation();

}

void AItem::PickUpOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);

	if (player != nullptr) {
		Destroy();
		if (isPoint) {

		}

		if (isMachineGun) {
			player->AddMachineGun(200);
			if (NS_Fire != nullptr) {
				//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_Fire, GetActorLocation(), FRotator::ZeroRotator);
				UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(NS_Fire, boxComp, NAME_None, GetActorLocation(), FRotator::ZeroRotator, EAttachLocation::Type::KeepRelativeOffset, true);
			}
		}

		if (isBomb) {
			player->AddBomb(10);
		}
	}
}

void AItem::AutoRotation()
{
	FRotator rot = GetActorRotation();


	SetActorRotation(FRotator(rot.Pitch, rot.Yaw +rotationSpeed, rot.Roll));
}
