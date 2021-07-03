// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/UserWidgetPool.h"

#include "ApBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UApBarWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* ApHorizBox;

	UPROPERTY(meta = (BindWidget))
	class USpacer* EndSpacer;

	public:
	void ResetBar(int TotalAp);
	void RefreshBar(int CurrentAp);
	void PreviewSpending(int AmountToPreview);
	void StopPreview();

	FUserWidgetPool Pool;
	int FullAmount = 0;

	
	
};
