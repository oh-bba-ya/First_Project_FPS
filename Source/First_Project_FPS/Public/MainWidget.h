// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class FIRST_PROJECT_FPS_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* scoreText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* scoreData;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* hpBar;
};
