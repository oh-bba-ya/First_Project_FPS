// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

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




}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AutoRotation(DeltaTime);

}

void AItem::PickUpOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AItem::AutoRotation(float deltaTime)
{
	rotationSpeed = deltaTime + rotationSpeed;
	FRotator rot = GetActorRotation();


	SetActorRotation(FRotator(rot.Pitch, rot.Yaw +rotationSpeed, rot.Roll));
}
