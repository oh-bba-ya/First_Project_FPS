// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPC.generated.h"

UCLASS()
class FIRST_PROJECT_FPS_API ANPC : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "NPC Settings")
		class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere, Category = "NPC Settings")
		class UStaticMeshComponent* meshComp;


	UPROPERTY(EditAnywhere, Category = "NPC Settings")
		TSubclassOf<class AItem> itemActor;

	UFUNCTION()
		void OnNPCOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);



};
