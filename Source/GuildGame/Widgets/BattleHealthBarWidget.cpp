// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleHealthBarWidget.h"

#include "CharacterSkillTooltipWidget.h"
#include "StatusEffectNodeWidget.h"
#include "StatusEffectStackableTooltipWidg.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "GuildGame/Managers/StatusEffectManager.h"
#include "GuildGame/Managers/TimedEventManager.h"
#include "GuildGame/Managers/WidgetManager.h"

void UBattleHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	StackableTooltip = Cast<UStatusEffectStackableTooltipWidg>(AWidgetManager::CreateWidgetInstance(EWidgetKeys::StatusEffectStackableTooltip, GetWorld()));

	StatusEffectNodePool.SetWorld(GetWorld());
	StatusEffectTooltipPool.SetWorld(GetWorld());
}

void UBattleHealthBarWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if(StatusEffectNodePool.IsInitialized())
	{
		StatusEffectNodePool.ReleaseAllSlateResources();
	}
	if(StatusEffectTooltipPool.IsInitialized())
	{
		StatusEffectTooltipPool.ReleaseAllSlateResources();
	}
}

void UBattleHealthBarWidget::SetHpBar(int CurrentHealth, int MaxHealth, int StartHealth)
{
	if(HealthBar == nullptr || HpText == nullptr)
		return;
	
	float Percentage = 0.0f;
	float StartPercent = 0.0f;

	if(MaxHealth > 0)
	{
		Percentage = (float)CurrentHealth / MaxHealth;
		StartPercent = (float)StartHealth / MaxHealth;
	}

	SetHpText(CurrentHealth, MaxHealth, StartHealth);
	if(CurrentHealth != StartHealth)
	{
		ATimedEventManager::LerpProgressBar(HealthBar, 0.4f, StartPercent, Percentage, GetWorld());
	}
	else
	{
		HealthBar->SetPercent(Percentage);
	}
}

void UBattleHealthBarWidget::SetArmorShieldBar(int CurrentArmor, int MaxArmor, int StartArmorShield)
{
	if(ArmorShieldBar == nullptr || ArmorShieldText == nullptr)
		return;
	
	float Percentage = 0.0f;
	float StartPercent = 0.0f;

	if(MaxArmor > 0)
	{
		Percentage = (float)CurrentArmor / MaxArmor;
		StartPercent = (float)StartArmorShield / MaxArmor;
	}

	SetArmorShieldText(CurrentArmor, MaxArmor, StartArmorShield);
	if(CurrentArmor != StartArmorShield)
	{
		ATimedEventManager::LerpProgressBar(ArmorShieldBar, 0.4f, StartPercent, Percentage, GetWorld());
	}
	else
	{
		if(CurrentArmor == 0.0f)
		{
			if(ArmorShieldPanel && ArmorShieldPanel->Visibility == ESlateVisibility::SelfHitTestInvisible && SeperatorPanel)
			{
				ArmorShieldPanel->SetVisibility(ESlateVisibility::Collapsed);
				SeperatorPanel->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
		
		ArmorShieldBar->SetPercent(Percentage);
	}
}

void UBattleHealthBarWidget::SetMagicShieldBar(int CurrentMagic, int MaxMagic, int StartMagicShield)
{
	if(MagicShieldBar == nullptr || MagicShieldText == nullptr)
		return;
	
	float Percentage = 0.0f;
	float StartPercent = 0.0f;

	if(MaxMagic > 0)
	{
		Percentage = (float)CurrentMagic / MaxMagic;
		StartPercent = (float)StartMagicShield / MaxMagic;
	}

	SetMagicShieldText(CurrentMagic, MaxMagic, StartMagicShield);
	if(CurrentMagic != StartMagicShield)
	{
		ATimedEventManager::LerpProgressBar(MagicShieldBar, 0.4f, StartPercent, Percentage, GetWorld());
	}
	else
	{
		if(CurrentMagic == 0.0f)
		{
			if(MagicShieldPanel && MagicShieldPanel->Visibility == ESlateVisibility::SelfHitTestInvisible && SeperatorPanel)
			{
				MagicShieldPanel->SetVisibility(ESlateVisibility::Collapsed);
				SeperatorPanel->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
		
		MagicShieldBar->SetPercent(Percentage);
	}
}

void UBattleHealthBarWidget::SetHpText(int CurrentHealth, int MaxHealth, int StartHealth)
{
	if(CurrentHealth != StartHealth)
	{
		FText FormatText = NSLOCTEXT("CommonWords", "HP", "{HP}");
		ATimedEventManager::LerpTextNumber(HpText, FormatText,"HP", 0.4f, StartHealth, CurrentHealth, GetWorld());
	}
	else
	{
		FString Hp = FString::Printf(TEXT("%d"), CurrentHealth);
		HpText->SetText(FText::FromString(Hp));
	}
}

void UBattleHealthBarWidget::SetMagicShieldText(int CurrentMagicShield, int MaxMagicShield, int StartMagicShield)
{
	if(CurrentMagicShield != StartMagicShield)
	{
		FText FormatText = NSLOCTEXT("CommonWords", "MS", "{MS}");
		ATimedEventManager::LerpTextNumber(MagicShieldText, FormatText,"MS", 0.4f, StartMagicShield, CurrentMagicShield, GetWorld());
	}
	else
	{
		FString Ms = FString::Printf(TEXT("%d"), CurrentMagicShield);
		MagicShieldText->SetText(FText::FromString(Ms));
	}
}

void UBattleHealthBarWidget::SetArmorShieldText(int CurrentArmorShield, int MaxArmorShield, int StartArmorShield)
{
	if(CurrentArmorShield != StartArmorShield)
	{
		FText FormatText = NSLOCTEXT("CommonWords", "AS", "{AS}");
		ATimedEventManager::LerpTextNumber(ArmorShieldText, FormatText,"AS", 0.4f, StartArmorShield, CurrentArmorShield, GetWorld());
	}
	else
	{
		FString As = FString::Printf(TEXT("%d"), CurrentArmorShield);
		ArmorShieldText->SetText(FText::FromString(As));
	}
}

void UBattleHealthBarWidget::SetStatusEffects(TArray<FStatusEffectData>* StatusEffects)
{
	if(StatusEffects == nullptr || StatusEffectsHorzBox == nullptr) return;

	for (int i = StatusEffectsHorzBox->GetChildrenCount() -1 ; i >= 0; --i)
	{
		UStatusEffectNodeWidget* ExistingNode = Cast<UStatusEffectNodeWidget>(StatusEffectsHorzBox->GetChildAt(i));
		if(ExistingNode)
		{
			if(ContainsStatusEffect(ExistingNode->StatusType, StatusEffects))
			{
				continue;
			}
			if(StatusEffectNodePool.IsInitialized())
			{
				StatusEffectNodePool.Release(ExistingNode);
			}
		}
	
		StatusEffectsHorzBox->RemoveChildAt(i);
	}

	for (int i = 0; i < StatusEffects->Num(); ++i)
	{
		UStatusEffectNodeWidget* ExistingNode = GetHorzBoxChildWithSameType((*StatusEffects)[i].Data.Type);
		if(ExistingNode == nullptr && StatusEffectNodePool.IsInitialized())
		{
			ExistingNode = Cast<UStatusEffectNodeWidget>(StatusEffectNodePool.GetOrCreateInstance(AWidgetManager::GetWidget(EWidgetKeys::StatusEffectNodeWidget, GetWorld())));
			StatusEffectsHorzBox->AddChildToHorizontalBox(ExistingNode);
		}

		if(ExistingNode)
		{
			FStatusEffectFileDataTable* StatusFile = StatusEffectManager::GetStatusEffectFile((*StatusEffects)[i].Data.Type, GetWorld());
			if(StatusFile)
			{
				ExistingNode->SetToolTip(StackableTooltip);
				ExistingNode->StatusTooltip = StackableTooltip;
				ExistingNode->TooltipPool = &StatusEffectTooltipPool;
				
				ExistingNode->SetStatusEffectNode(&((*StatusEffects)[i]), StatusFile);
				if (ExistingNode->StatusImage && ExistingNode->TurnText)
				{
					ExistingNode->StatusImage->SetBrushResourceObject(StatusFile->Image);
					ExistingNode->TurnText->SetText(FText::AsNumber((*StatusEffects)[i].Data.RemainingTurns));
				}
			}
		}
	}
	
	//StackStatusEffect(&((*StatusEffects)[i]));
}

bool UBattleHealthBarWidget::ContainsStatusEffect(EEffectType TypeToSearch, TArray<FStatusEffectData>* StatusEffects)
{
	if(StatusEffects == nullptr)
		return false;

	
	for (int i = 0; i < StatusEffects->Num(); ++i)
	{
		if((*StatusEffects)[i].Data.Type == TypeToSearch)
		{
			return  true;
		}
	}
	return  false;
}

UStatusEffectNodeWidget* UBattleHealthBarWidget::GetHorzBoxChildWithSameType(EEffectType TypeToSearch)
{
	if(StatusEffectsHorzBox == nullptr)
		return nullptr;

	for (int i = 0; i < StatusEffectsHorzBox->GetChildrenCount(); ++i)
	{
		UStatusEffectNodeWidget* ExistingNode = Cast<UStatusEffectNodeWidget>(StatusEffectsHorzBox->GetChildAt(i));
		if(ExistingNode && ExistingNode->StatusType == TypeToSearch)
		{
			return  ExistingNode;
		}
	}
	return  nullptr;
}

void UBattleHealthBarWidget::SetHpDamagePreviewBar(float DamageToPreview, int MaxHealth)
{
	if(PreviewBar == nullptr || HealthBar == nullptr || HpSizeBox == nullptr || MaxHealth == 0) return;


	float CalculatedCurrentHP = MaxHealth * HealthBar->Percent;
	float DamagePerc = DamageToPreview/MaxHealth;

	DamagePerc = FMath::Clamp(DamagePerc,0.0f, HealthBar->Percent);
	
	PreviewBar->SetPercent(DamagePerc);
	FVector2D RenderTranslition = PreviewBar->RenderTransform.Translation;
	RenderTranslition.X = HpSizeBox->WidthOverride * (HealthBar->Percent - DamagePerc);
	PreviewBar->SetRenderTranslation(RenderTranslition);
	PreviewBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	ATimedEventManager::LerpWidgetOpacity(PreviewBar, 0.75f, 1, 0, true,  GetWorld());

	float PreviewHP = (int)FMath::Clamp(CalculatedCurrentHP - DamageToPreview, 0.0f, CalculatedCurrentHP);
	
	SetHpText(PreviewHP, MaxHealth, PreviewHP);
}

void UBattleHealthBarWidget::ResetHpDamagePreviewBar(int MaxHealth)
{
	if(PreviewBar == nullptr || HealthBar == nullptr) return;

	PreviewBar->SetVisibility(ESlateVisibility::Hidden);
	ATimedEventManager::RemoveWidgetOpacityTimer(PreviewBar);

	float CalculatedCurrentHP = MaxHealth * HealthBar->Percent;
	SetHpText(CalculatedCurrentHP, MaxHealth, CalculatedCurrentHP);
}

void UBattleHealthBarWidget::SetArmorDamagePreviewBar(float DamageToPreview, int MaxArmor)
{
	if(ArmorShieldPreviewBar == nullptr || ArmorShieldBar == nullptr || ArmorShieldPanel == nullptr || MaxArmor == 0 ||
		MagicShieldPanel == nullptr || SeperatorPanel == nullptr || ArmorSizeBox == nullptr) return;

	UHorizontalBoxSlot* MagicSlot = Cast<UHorizontalBoxSlot>(MagicShieldPanel->Slot);
	UHorizontalBoxSlot* ArmorSlot = Cast<UHorizontalBoxSlot>(ArmorShieldPanel->Slot);
	UHorizontalBoxSlot* SeperatorSlot = Cast<UHorizontalBoxSlot>(SeperatorPanel->Slot);

	float HorizChildRatioSum = 0; 
	if(MagicSlot && ArmorSlot && SeperatorSlot)
	{
		if(MagicShieldPanel->Visibility != ESlateVisibility::Collapsed)
		{
			HorizChildRatioSum += MagicSlot->Size.Value;
		}
		if(ArmorShieldPanel->Visibility != ESlateVisibility::Collapsed)
		{
			HorizChildRatioSum += ArmorSlot->Size.Value;
		}
		if(SeperatorPanel->Visibility != ESlateVisibility::Collapsed)
		{
			HorizChildRatioSum += SeperatorSlot->Size.Value;
		}
	}

	if(MagicSlot && ArmorSlot && SeperatorSlot && HorizChildRatioSum > 0.0f)
	{
		float Width = (ArmorSizeBox->WidthOverride / HorizChildRatioSum) * ArmorSlot->Size.Value;
		float CalculatedCurrentAS = MaxArmor * ArmorShieldBar->Percent;
		float DamagePerc = DamageToPreview/MaxArmor;

		DamagePerc = FMath::Clamp(DamagePerc,0.0f, ArmorShieldBar->Percent);
		
		ArmorShieldPreviewBar->SetPercent(DamagePerc);
		FVector2D RenderTranslition = ArmorShieldPreviewBar->RenderTransform.Translation;
		RenderTranslition.X = Width * (ArmorShieldBar->Percent - DamagePerc);
		ArmorShieldPreviewBar->SetRenderTranslation(RenderTranslition);
		ArmorShieldPreviewBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ATimedEventManager::LerpWidgetOpacity(ArmorShieldPreviewBar, 0.75f, 1, 0, true,  GetWorld());

		float PreviewAS = (int)FMath::Clamp(CalculatedCurrentAS - DamageToPreview, 0.0f, CalculatedCurrentAS);
		SetArmorShieldText(PreviewAS, MaxArmor, PreviewAS);
	}
}

void UBattleHealthBarWidget::ResetArmorDamagePreviewBar(int MaxArmor)
{
	if(ArmorShieldPreviewBar == nullptr || ArmorShieldBar == nullptr) return;

	ArmorShieldPreviewBar->SetVisibility(ESlateVisibility::Hidden);
	ATimedEventManager::RemoveWidgetOpacityTimer(ArmorShieldPreviewBar);

	float CalculatedCurrentAS = MaxArmor * ArmorShieldBar->Percent;
	SetArmorShieldText(CalculatedCurrentAS, MaxArmor, CalculatedCurrentAS);
}

void UBattleHealthBarWidget::SetMagicArmorDamagePreviewBar(float DamageToPreview, int MaxMagicArmor)
{
	if(MagicShieldPreviewBar == nullptr || MagicShieldBar == nullptr || MagicShieldPanel == nullptr || MaxMagicArmor == 0 ||
		ArmorShieldPanel == nullptr || SeperatorPanel == nullptr || ArmorSizeBox == nullptr) return;

	UHorizontalBoxSlot* MagicSlot = Cast<UHorizontalBoxSlot>(MagicShieldPanel->Slot);
	UHorizontalBoxSlot* ArmorSlot = Cast<UHorizontalBoxSlot>(ArmorShieldPanel->Slot);
	UHorizontalBoxSlot* SeperatorSlot = Cast<UHorizontalBoxSlot>(SeperatorPanel->Slot);

	float HorizChildRatioSum = 0; 
	if(MagicSlot && ArmorSlot && SeperatorSlot)
	{
		if(MagicShieldPanel->Visibility != ESlateVisibility::Collapsed)
		{
			HorizChildRatioSum += MagicSlot->Size.Value;
		}
		if(ArmorShieldPanel->Visibility != ESlateVisibility::Collapsed)
		{
			HorizChildRatioSum += ArmorSlot->Size.Value;
		}
		if(SeperatorPanel->Visibility != ESlateVisibility::Collapsed)
		{
			HorizChildRatioSum += SeperatorSlot->Size.Value;
		}
	}

	if(MagicSlot && ArmorSlot && SeperatorSlot && HorizChildRatioSum > 0.0f)
	{
		float Width = (ArmorSizeBox->WidthOverride / HorizChildRatioSum) * MagicSlot->Size.Value;
		float CalculatedCurrentMS = MaxMagicArmor * MagicShieldBar->Percent;
		float DamagePerc = DamageToPreview/MaxMagicArmor;

		DamagePerc = FMath::Clamp(DamagePerc,0.0f, MagicShieldBar->Percent);

		MagicShieldPreviewBar->SetPercent(DamagePerc);
		FVector2D RenderTranslition = MagicShieldPreviewBar->RenderTransform.Translation;
		RenderTranslition.X = Width * (MagicShieldBar->Percent - DamagePerc);
		MagicShieldPreviewBar->SetRenderTranslation(RenderTranslition);
		MagicShieldPreviewBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ATimedEventManager::LerpWidgetOpacity(MagicShieldPreviewBar, 0.75f, 1, 0, true,  GetWorld());

		float PreviewMS = (int)FMath::Clamp(CalculatedCurrentMS - DamageToPreview, 0.0f, CalculatedCurrentMS);
		SetMagicShieldText(PreviewMS, MaxMagicArmor, PreviewMS);
	}
}

void UBattleHealthBarWidget::ResetMagicArmorDamagePreviewBar(int MaxMagicArmor)
{
	if(MagicShieldPreviewBar == nullptr || MagicShieldBar == nullptr) return;

	MagicShieldPreviewBar->SetVisibility(ESlateVisibility::Hidden);
	ATimedEventManager::RemoveWidgetOpacityTimer(MagicShieldPreviewBar);

	float CalculatedCurrentMS = MaxMagicArmor * MagicShieldBar->Percent;
	SetMagicShieldText(CalculatedCurrentMS, MaxMagicArmor, CalculatedCurrentMS);
}
