// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "HpBar.generated.h"

/**
 * 
 */
UCLASS()
class FIRST_PROJECT_FPS_API UHpBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UProgressBar* hpBar;
	
};
