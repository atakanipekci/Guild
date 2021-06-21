// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleHealthBarWidget.h"

#include "CharacterSkillTooltipWidget.h"
#include "StatusEffectNodeWidget.h"
#include "StatusEffectStackableTooltipWidg.h"
#include "Components/CanvasPanelSlot.h"
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
	Tooltip = Cast<UStatusEffectStackableTooltipWidg>(WidgetManager::CreateWidgetInstance(EWidgetKeys::StatusEffectStackableTooltip, GetWorld()));
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
	if(StatusEffects == nullptr) return;

	HorzBoxChildrenIndex = 0;


	for (int i = 0; i < StatusEffects->Num(); ++i)
	{
		if(StatusEffectsHorzBox)
		{
			UStatusEffectNodeWidget* NodeInstance = nullptr;

			UStatusEffectNodeWidget* StackableNode = GetStatusEffectNodeWithSameType((*StatusEffects)[i].Type);

			bool bIstack = false;

			if(StackableNode == nullptr)
			{
				if(HorzBoxChildrenIndex < HorzBoxChildren.Num())
				{
					NodeInstance = HorzBoxChildren[HorzBoxChildrenIndex];
					HorzBoxChildrenIndex++;
					StatusEffectsHorzBox->AddChildToHorizontalBox(NodeInstance);
				}
				else
				{
					NodeInstance = Cast<UStatusEffectNodeWidget>(WidgetManager::CreateWidgetInstance(EWidgetKeys::StatusEffectNodeWidget, this));
					StatusEffectsHorzBox->AddChildToHorizontalBox(NodeInstance);
					HorzBoxChildren.Add(NodeInstance);
					HorzBoxChildrenIndex++;
				}
			}
			else
			{
				NodeInstance = StackableNode;
				bIstack = true;
			}


			if(NodeInstance)
			{
				if(bIstack == false)
				{
					FStatusEffectFileDataTable* StatusFile = StatusEffectManager::GetStatusEffectFile((*StatusEffects)[i].Type, GetWorld());
					NodeInstance->SetToolTip(Tooltip);
					NodeInstance->SetStatusEffectNode(&((*StatusEffects)[i]), StatusFile, Tooltip, &TooltipChildInstances);

					if(StatusFile)
					{
						if (NodeInstance->StatusImage && NodeInstance->TurnText)
						{
							NodeInstance->StatusImage->SetBrushResourceObject(StatusFile->Image);
							NodeInstance->TurnText->SetText(FText::AsNumber((*StatusEffects)[i].RemainingTurns));
						}
					}
				}
				else
				{
					NodeInstance->StackStatusEffect(&((*StatusEffects)[i]));
				}
			}
		}
	}
	
	if(StatusEffectsHorzBox)
	{
		for (int i = HorzBoxChildren.Num() - 1; i >= HorzBoxChildrenIndex; --i)
		{
			if(StatusEffectsHorzBox->HasChild(HorzBoxChildren[i]))
			{
				StatusEffectsHorzBox->RemoveChild(HorzBoxChildren[i]);
			}
		}
	}
}

UStatusEffectNodeWidget* UBattleHealthBarWidget::GetStatusEffectNodeWithSameType(EStatusEffectType TypeToSearch)
{
	for (int i = HorzBoxChildrenIndex - 1; i >= 0; --i)
	{
		if(i < HorzBoxChildren.Num())
		{
			if(HorzBoxChildren[i] && HorzBoxChildren[i]->StatusType == TypeToSearch)
			{
				return  HorzBoxChildren[i];
			}
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
