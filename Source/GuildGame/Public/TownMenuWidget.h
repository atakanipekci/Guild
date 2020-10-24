// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TownMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UTownMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	protected:

	UPROPERTY(meta = (BindWidget))
	class UButton* TestButton;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TestTextBlock;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* GoldText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* RecruitGoldTxt;
	UPROPERTY(meta = (BindWidget))
	UButton* BuildRecruitBtn;

	UPROPERTY(EditAnywhere, Category = "UI")
	int RecruitBuildingPrice = 250;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnTestClicked();
	UFUNCTION()
	void OnBuildRecruit();

	public:
	UFUNCTION()
	void UpdateUI();
	/*UFUNCTION()
	void OnBuildYes();
	UFUNCTION()
	void OnBuildNo();*/
};
