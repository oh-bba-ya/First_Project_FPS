// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerCharacter.h"
#include "Item.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(70, 70, 70));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(boxComp);
	meshComp->SetRelativeLocation(FVector(0, 0, -50));

}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ANPC::OnNPCOverlapBegin);
	
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANPC::OnNPCOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
	if (player != nullptr) {
		GetWorld()->SpawnActor<AItem>(itemActor, GetActorLocation(), GetActorRotation());
		UE_LOG(LogTemp, Warning, TEXT("NPC Collision"));
		Destroy();
	}
}

