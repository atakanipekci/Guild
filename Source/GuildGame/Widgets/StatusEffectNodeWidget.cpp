// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffectNodeWidget.h"
#include "CharacterSkillTooltipWidget.h"
#include "StatusEffectStackableTooltipWidg.h"
#include "StatusEffectTooltipWidget.h"
#include "Components/VerticalBox.h"
#include "GuildGame/Managers/StatusEffectManager.h"
#include "GuildGame/Managers/WidgetManager.h"

void UStatusEffectNodeWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if(StatusTooltip && StatusTooltip->StatusNodeVerticalBox && StatusFile && StatusEffectData.Num() > 0 )
	{
		for (int i = StatusTooltip->StatusNodeVerticalBox->GetChildrenCount() - 1; i >= 0; --i)
		{
			UStatusEffectTooltipWidget* TooltipWidget = Cast<UStatusEffectTooltipWidget>(StatusTooltip->StatusNodeVerticalBox->GetChildAt(i));
			if(TooltipPool && TooltipPool->IsInitialized() && TooltipWidget)
			{
				TooltipPool->Release(TooltipWidget);
				StatusTooltip->StatusNodeVerticalBox->RemoveChildAt(i);
			}
		}
		
		for (int i = 0; i < StatusEffectData.Num(); ++i)
		{
			if(StatusEffectData[i] != nullptr && TooltipPool && TooltipPool->IsInitialized())
			{
				UStatusEffectTooltipWidget* CreatedTooltipChild = Cast<UStatusEffectTooltipWidget>(TooltipPool->GetOrCreateInstance(AWidgetManager::GetWidget(EWidgetKeys::StatusEffectTooltip, GetWorld())));
				if(CreatedTooltipChild)
				{
					CreatedTooltipChild->Refresh(*StatusFile, *StatusEffectData[i]);
					StatusTooltip->StatusNodeVerticalBox->AddChild(CreatedTooltipChild);
				}
			}
		}
	}
}

void UStatusEffectNodeWidget::SetStatusEffectNode(const struct FStatusEffectData* Data, const struct FStatusEffectFileDataTable* File)
{
	if(File == nullptr || Data == nullptr) return;
	this->StatusFile = File;
	this->StatusEffectData.Empty();
	this->StatusEffectData.Add(Data);
	this->StatusType = Data->Data.Type;
}

void UStatusEffectNodeWidget::StackStatusEffect(struct FStatusEffectData* Data)
{
	if(Data == nullptr) return;
	this->StatusEffectData.Add(Data);
}
