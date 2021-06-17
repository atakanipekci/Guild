// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GuildGame/Managers/StatusEffectManager.h"

#include "BattleHealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UBattleHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
	public:
	virtual void NativeConstruct() override;
	void SetHpBar(int GetCurrentHealth, int GetMaxHealth);
	void SetStatusEffects(TArray<FStatusEffectData>* StatusEffects);
	class UStatusEffectNodeWidget* GetNodeWithSameType(EStatusEffectType TypeToSearch);

	UPROPERTY()
	class UStatusEffectStackableTooltipWidg* Tooltip;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HpText;

	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* StatusEffectsHorzBox;

	UPROPERTY()
	TArray<class UStatusEffectNodeWidget*> HorzBoxChildren;
	int HorzBoxChildrenIndex = 0;

	UPROPERTY()
	TArray<class UStatusEffectTooltipWidget*> TooltipChildInstances;

};
