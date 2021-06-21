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
	void SetHpBar(int CurrentHealth, int MaxHealth, int StartHealth);
	void SetHpText(int CurrentHealth, int MaxHealth, int StartHealth);
	void SetStatusEffects(TArray<FStatusEffectData>* StatusEffects);
	class UStatusEffectNodeWidget* GetStatusEffectNodeWithSameType(EStatusEffectType TypeToSearch);

	void SetDamagePreviewBar(float DamageToPreview, int MaxHealth);
	void ResetDamagePreviewBar(int MaxHealth);


	UPROPERTY()
	class UStatusEffectStackableTooltipWidg* Tooltip;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PreviewBar;

	UPROPERTY(meta = (BindWidget))
	class USizeBox* HpSizeBox;
	
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
