// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartWidget.generated.h"

/**
 * 
 */
UCLASS()
class FIRST_PROJECT_FPS_API UStartWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;


	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* btn_Start;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* btn_Quit;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* text_Title;



private:
	UFUNCTION()
	void StartGame();

	UFUNCTION()
	void Quit();
	
};
