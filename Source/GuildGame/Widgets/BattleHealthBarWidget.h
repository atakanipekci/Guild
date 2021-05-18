// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleHealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UBattleHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
	public:
	void SetHpBar(int GetCurrentHealth, int GetMaxHealth);
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HpText;
	
};
