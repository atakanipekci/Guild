// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/UserWidgetPool.h"

#include "BattleHealthBarWidget.generated.h"

enum class EEffectType : uint8;

struct FStatusEffectData;
/**
 * 
 */
UCLASS()
class GUILDGAME_API UBattleHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
	public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	void SetHpBar(int CurrentHealth, int MaxHealth, int StartHealth);
	void SetArmorShieldBar(int CurrentArmor, int MaxArmor, int StartArmorShield);
	void SetMagicShieldBar(int CurrentMagic, int MaxMagic, int StartMagicShield);
	void SetHpText(int CurrentHealth, int MaxHealth, int StartHealth);
	void SetArmorShieldText(int CurrentArmorShield, int MaxArmorShield, int StartArmorShield);
	void SetMagicShieldText(int CurrentMagicShield, int MaxMagicShield, int StartMagicShield);
	void SetStatusEffects(TArray<FStatusEffectData>* StatusEffects);
	bool ContainsStatusEffect(EEffectType TypeToSearch, TArray<FStatusEffectData>* StatusEffects);
	class UStatusEffectNodeWidget* GetHorzBoxChildWithSameType(EEffectType TypeToSearch);

	void SetHpDamagePreviewBar(float DamageToPreview, int MaxHealth);
	void ResetHpDamagePreviewBar(int MaxHealth);

	void SetArmorDamagePreviewBar(float DamageToPreview, int MaxArmor);
	void ResetArmorDamagePreviewBar(int MaxArmor);

	void SetMagicArmorDamagePreviewBar(float DamageToPreview, int MaxMagicArmor);
	void ResetMagicArmorDamagePreviewBar(int MaxMagicArmor);


	UPROPERTY()
	class UStatusEffectStackableTooltipWidg* StackableTooltip;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PreviewBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ArmorShieldBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ArmorShieldPreviewBar;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* ArmorShieldPanel;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* MagicShieldBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* MagicShieldPreviewBar;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* MagicShieldPanel;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* SeperatorPanel;

	UPROPERTY(meta = (BindWidget))
	class USizeBox* HpSizeBox;
	
	UPROPERTY(meta = (BindWidget))
	class USizeBox* ArmorSizeBox;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HpText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ArmorShieldText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MagicShieldText;

	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* StatusEffectsHorzBox;

	FUserWidgetPool StatusEffectNodePool;
	FUserWidgetPool StatusEffectTooltipPool;

	UPROPERTY()
	TArray<class UStatusEffectTooltipWidget*> TooltipChildInstances;

};
