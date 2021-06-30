// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleHealthBarWidget.h"

#include "CharacterSkillTooltipWidget.h"
#include "StatusEffectNodeWidget.h"
#include "StatusEffectStackableTooltipWidg.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
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
	if(MaxHealth <= 0 || HealthBar == nullptr || HpText == nullptr)
		return;
	
	const float Percentage = (float)CurrentHealth / MaxHealth;
	const float StartPercent = (float)StartHealth / MaxHealth;

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
		UStatusEffectNodeWidget* ExistingNode = GetHorzBoxChildWithSameType((*StatusEffects)[i].Type);
		if(ExistingNode == nullptr && StatusEffectNodePool.IsInitialized())
		{
			ExistingNode = Cast<UStatusEffectNodeWidget>(StatusEffectNodePool.GetOrCreateInstance(AWidgetManager::GetWidget(EWidgetKeys::StatusEffectNodeWidget, GetWorld())));
			StatusEffectsHorzBox->AddChildToHorizontalBox(ExistingNode);
		}

		if(ExistingNode)
		{
			FStatusEffectFileDataTable* StatusFile = StatusEffectManager::GetStatusEffectFile((*StatusEffects)[i].Type, GetWorld());
			if(StatusFile)
			{
				ExistingNode->SetToolTip(StackableTooltip);
				ExistingNode->StatusTooltip = StackableTooltip;
				ExistingNode->TooltipPool = &StatusEffectTooltipPool;
				
				ExistingNode->SetStatusEffectNode(&((*StatusEffects)[i]), StatusFile);
				if (ExistingNode->StatusImage && ExistingNode->TurnText)
				{
					ExistingNode->StatusImage->SetBrushResourceObject(StatusFile->Image);
					ExistingNode->TurnText->SetText(FText::AsNumber((*StatusEffects)[i].RemainingTurns));
				}
			}
		}
	}
	
	//StackStatusEffect(&((*StatusEffects)[i]));
}

bool UBattleHealthBarWidget::ContainsStatusEffect(EStatusEffectType TypeToSearch, TArray<FStatusEffectData>* StatusEffects)
{
	if(StatusEffects == nullptr)
		return false;

	
	for (int i = 0; i < StatusEffects->Num(); ++i)
	{
		if((*StatusEffects)[i].Type == TypeToSearch)
		{
			return  true;
		}
	}
	return  false;
}

UStatusEffectNodeWidget* UBattleHealthBarWidget::GetHorzBoxChildWithSameType(EStatusEffectType TypeToSearch)
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

void UBattleHealthBarWidget::SetDamagePreviewBar(float DamageToPreview, int MaxHealth)
{
	if(PreviewBar == nullptr || HealthBar == nullptr || HpSizeBox == nullptr || MaxHealth == 0) return;


	float CalculatedCurrentHP = MaxHealth * HealthBar->Percent;
	float DamagePerc = DamageToPreview/MaxHealth;

	DamagePerc = FMath::Clamp(DamagePerc,0.0f, HealthBar->Percent);
	
	PreviewBar->SetPercent(DamagePerc);

	// Set(HpSizeBox->WidthOverride * (HealthBar->Percent - DamagePerc), PreviewBar->RenderTransform.Translation.Y);
	FVector2D RenderTranslition = PreviewBar->RenderTransform.Translation;
	RenderTranslition.X = HpSizeBox->WidthOverride * (HealthBar->Percent - DamagePerc);
	PreviewBar->SetRenderTranslation(RenderTranslition);
	
	// UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(PreviewBar->Slot);
	// if(CanvasSlot && DamagePerc > 0.0f)
	// {	
	// 	UE_LOG(LogTemp, Warning, TEXT("HpSizeBox->WidthOverride %f"), HpSizeBox->WidthOverride * (HealthBar->Percent - DamagePerc));
	// 	FMargin Margin;
	// 	Margin.Left += HpSizeBox->WidthOverride * (HealthBar->Percent - DamagePerc);
	// 	CanvasSlot->SetOffsets(Margin);
	// }
	PreviewBar->SetVisibility(ESlateVisibility::Visible);
	ATimedEventManager::LerpWidgetOpacity(PreviewBar, 0.75f, 1, 0, true,  GetWorld());

	float PreviewHP = (int)CalculatedCurrentHP - (int)DamageToPreview;
	SetHpText(PreviewHP, MaxHealth, PreviewHP);
}

void UBattleHealthBarWidget::ResetDamagePreviewBar(int MaxHealth)
{
	if(PreviewBar == nullptr || HealthBar == nullptr) return;

	PreviewBar->SetVisibility(ESlateVisibility::Hidden);
	ATimedEventManager::RemoveWidgetOpacityTimer(PreviewBar);

	float CalculatedCurrentHP = MaxHealth * HealthBar->Percent;
	SetHpText(CalculatedCurrentHP, MaxHealth, CalculatedCurrentHP);
}
