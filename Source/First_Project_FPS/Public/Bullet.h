// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class FIRST_PROJECT_FPS_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// �߻�ü�� �̵��� ����� ������Ʈ
	UPROPERTY(VisibleAnywhere, Category = Movement)
		class UProjectileMovementComponent* movementComp;

	// �浹 ������Ʈ
	UPROPERTY(VisibleAnywhere, Category = Collision)
		class USphereComponent* collisionComp;

	// �ܰ� ������Ʈ
	UPROPERTY(VisibleAnywhere, Category = Collision)
		class UStaticMeshComponent* bodyMeshComp;

	// �Ѿ� ���� �Լ� (Destory �̿�)
	void Die();

	UFUNCTION()
	void OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UPROPERTY(EditAnywhere, Category = Niagara)
	class UNiagaraSystem* NS_Explosion;

	UPROPERTY(EditAnywhere, Category = Niagara)
	class USoundBase* SB_Explosion;

private:

	UPROPERTY(EditAnywhere, Category = Damage)
	int damage = 2;
};
