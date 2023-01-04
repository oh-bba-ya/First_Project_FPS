// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include <GameFramework/CharacterMovementComponent.h>



void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// �÷��̾��� �̵��ӵ��� ������ Speed�� �Ҵ��ϰ� �ʹ�.
	// 1. ���� ĳ���� ��������
	//auto ownerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	auto ownerCharacter = TryGetPawnOwner();

	// 2. �÷��̾�� ĳ����
	auto player = Cast<APlayerCharacter>(ownerCharacter);

	if (player != nullptr) {
		// 3. �̵��ӵ��� �ʿ��ϴ�.
		FVector Velocity = player->GetVelocity();
		float vlength = Velocity.Length();

		speed = vlength;


		// �÷��̾ ���� ���߿� �ִ��� ���θ� ����� �ʹ�.
		auto movement = player->GetCharacterMovement();
		isInAir = movement->IsFalling();

	}
}
