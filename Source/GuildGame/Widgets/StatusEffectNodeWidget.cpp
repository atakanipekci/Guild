// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffectNodeWidget.h"
#include "CharacterSkillTooltipWidget.h"
#include "StatusEffectStackableTooltipWidg.h"
#include "StatusEffectTooltipWidget.h"
#include "Components/VerticalBox.h"
#include "GuildGame/Managers/WidgetManager.h"

void UStatusEffectNodeWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if(StatusTooltip && StatusTooltip->StatusNodeVerticalBox && StatusFile && StatusEffectData.Num() > 0 && TooltipChildInstances)
	{
		StatusTooltip->StatusNodeVerticalBox->ClearChildren();
		for (int i = 0; i < StatusEffectData.Num(); ++i)
		{
			if(StatusEffectData[i] != nullptr && StatusFile != nullptr)
			{
				if(i < (*TooltipChildInstances).Num() && (*TooltipChildInstances)[i] != nullptr)
				{
					StatusTooltip->StatusNodeVerticalBox->AddChild((*TooltipChildInstances)[i]);
					(*TooltipChildInstances)[i]->Refresh(*StatusFile, *StatusEffectData[i]);
				}
				else
				{
					UStatusEffectTooltipWidget* CreatedTooltipChild = Cast<UStatusEffectTooltipWidget>(WidgetManager::CreateWidgetInstance(EWidgetKeys::StatusEffectTooltip, GetWorld()));
					if(CreatedTooltipChild)
					{
						(*TooltipChildInstances).Add(CreatedTooltipChild);
						CreatedTooltipChild->Refresh(*StatusFile, *StatusEffectData[i]);
						StatusTooltip->StatusNodeVerticalBox->AddChild(CreatedTooltipChild);
					}
				}
			}
		}
	}
}

void UStatusEffectNodeWidget::SetStatusEffectNode(struct FStatusEffectData* Data, struct FStatusEffectFileDataTable* File, class UStatusEffectStackableTooltipWidg* Tooltip, TArray<class UStatusEffectTooltipWidget*>* ChildInstances)
{
	if(File == nullptr || Data == nullptr) return;
	this->StatusFile = File;
	this->StatusEffectData.Empty();
	this->StatusEffectData.Add(Data);
	this->StatusTooltip = Tooltip;
	this->StatusType = Data->Type;
	TooltipChildInstances = ChildInstances;
}

void UStatusEffectNodeWidget::StackStatusEffect(struct FStatusEffectData* Data)
{
	if(Data == nullptr) return;
	this->StatusEffectData.Add(Data);
}
