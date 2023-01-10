// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EnemySimple.h"
#include "PlayerCharacter.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* characterActor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass());

	if (characterActor != nullptr)
	{
		character = Cast<APlayerCharacter>(characterActor);

		if (character != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("not null : character"))
		}
	}


}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::EnemyHitEvent(int32 d)
{
	if (enemyHP > 0) {
		enemyHP -= d;
		UE_LOG(LogTemp, Warning, TEXT("Enemy HP :  %d"), enemyHP);

	}

	if (enemyHP <= 0) {
		Destroy();
	}
	/*
	else {
		Destroy();
	}
	*/
}



