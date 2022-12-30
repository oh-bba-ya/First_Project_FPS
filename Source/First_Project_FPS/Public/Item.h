// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class FIRST_PROJECT_FPS_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Item Settings")
	class UBoxComponent* boxComp;


	UPROPERTY(EditAnywhere, Category = "Item Settings")
	class UStaticMeshComponent* meshComp;

	UFUNCTION()
	void PickUpOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void AutoRotation();

	UPROPERTY(EditAnyWhere, Category = "Item Settings")
	float rotationSpeed = 1;


	// ÃÑ¾ÆÀÌÅÛ
	UPROPERTY(EditAnyWhere, Category = "Item Settings")
		bool isMachineGun = false;

	// ÆøÅº
	UPROPERTY(EditAnyWhere, Category = "Item Settings")
		bool isBomb = false;


	// Á¡¼ö
	UPROPERTY(EditAnyWhere, Category = "Item Settings")
		bool isPoint = false;


};
