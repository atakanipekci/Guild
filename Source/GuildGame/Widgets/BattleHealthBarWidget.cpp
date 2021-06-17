// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleHealthBarWidget.h"

#include "CharacterSkillTooltipWidget.h"
#include "StatusEffectNodeWidget.h"
#include "StatusEffectStackableTooltipWidg.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GuildGame/Managers/WidgetManager.h"

void UBattleHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Tooltip = Cast<UStatusEffectStackableTooltipWidg>(WidgetManager::CreateWidgetInstance(EWidgetKeys::StatusEffectStackableTooltip, GetWorld()));
}

void UBattleHealthBarWidget::SetHpBar(int CurrentHealth, int MaxHealth)
{
	if(MaxHealth <= 0 || HealthBar == nullptr || HpText == nullptr)
		return;
	
	const float Percentage = (float)CurrentHealth / MaxHealth;

	HealthBar->SetPercent(Percentage);
	FString Hp = FString::Printf(TEXT("%d"), CurrentHealth);
	HpText->SetText(FText::FromString(Hp));
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

			UStatusEffectNodeWidget* StackableNode = GetNodeWithSameType((*StatusEffects)[i].Type);

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

UStatusEffectNodeWidget* UBattleHealthBarWidget::GetNodeWithSameType(EStatusEffectType TypeToSearch)
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
