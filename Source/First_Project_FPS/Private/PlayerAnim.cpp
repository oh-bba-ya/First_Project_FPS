// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include <GameFramework/CharacterMovementComponent.h>



void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 플레이어의 이동속도를 가져와 Speed에 할당하고 싶다.
	// 1. 소유 캐릭터 가져오기
	//auto ownerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	auto ownerCharacter = TryGetPawnOwner();

	// 2. 플레이어로 캐스팅
	auto player = Cast<APlayerCharacter>(ownerCharacter);

	if (player != nullptr) {
		// 3. 이동속도가 필요하다.
		FVector Velocity = player->GetVelocity();
		float vlength = Velocity.Length();

		speed = vlength;


		// 플레이어가 현재 공중에 있는지 여부를 기억학 싶다.
		auto movement = player->GetCharacterMovement();
		isInAir = movement->IsFalling();

	}
}
