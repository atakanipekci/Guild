// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleHudWidget.h"

#include "BattleCharSkillNodeWidget.h"
#include "CharacterSkillTooltipWidget.h"
#include "GuildGameInstance.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "GuildGame/Characters/GGCharacter.h"
#include "GuildGame/Managers/WidgetManager.h"
#include "GuildGame/Skills/CharacterSkill.h"
#include "Kismet/GameplayStatics.h"

void UBattleHudWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Tooltip = CreateWidget<UCharacterSkillTooltipWidget>(GetWorld(), WidgetManager::GetWidget(EWidgetKeys::CharacterSkillTooltip));
	
}

void UBattleHudWidget::RefreshSkillsArray(AGGCharacter* SelectedChar)
{
	if(SelectedChar == nullptr)
		return;

	LatestSelectedChar = SelectedChar;

	if(ApText)
	{
		ApText->SetText(FText::AsNumber(SelectedChar->GetCurrentAP()));
	}
	
    if(SkillNodesGrid && SkillsPanel)
	{
        SkillNodes.Empty();
    	UGuildGameInstance* GameInstance = Cast<UGuildGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	    if(GameInstance == nullptr || GameInstance->CharacterSkillsFileTable == nullptr)
	    {
	        return;
	    }

    	TArray<class CharacterSkill*>* Skills = SelectedChar->GetSkills();

    	FCharacterDelegate Delegate;
		Delegate.BindDynamic(this, &UBattleHudWidget::OnApChange);
    	SelectedChar->RefreshHudOnApChangeDelegate = Delegate;


		for (int i = 0; i < SkillNodesGrid->GetChildrenCount(); ++i)
		{
			UBattleCharSkillNodeWidget* ChildWidget = Cast<UBattleCharSkillNodeWidget>(SkillNodesGrid->GetChildAt(i));
			if(ChildWidget && GameInstance)
			{
				if(Skills && i - 1 < Skills->Num() && i >= 1 && Skills->Num() > 0)
				{
					CharacterSkill* Skill = (*Skills)[i - 1];
					if(Skill)
					{
						FSkillData* SkillData = &(Skill->GetSkillData());
						FCharSkillFileDataTable* SkillFile =  &(Skill->GetSkillFiles());

						if(SkillData && SkillFile)
						{
							ChildWidget->SetVisibility(ESlateVisibility::Visible);
							ChildWidget->SkillsData = *SkillData;
							ChildWidget->SkillTexture = SkillFile->SkillImage;
							ChildWidget->DescText = SkillFile->Desc;

							SkillNodes.Add(ChildWidget);
							ChildWidget->SetToolTip(Tooltip);

							ChildWidget->Tooltip = Tooltip;

							ChildWidget->RefreshNode(SelectedChar, this);
						}
					}
				}
				else
				{
					if(i == 0)
					{
						//Movement Skill
						ChildWidget->SetVisibility(ESlateVisibility::Visible);

						SkillNodes.Add(ChildWidget);
						ChildWidget->SetToolTip(Tooltip);
						ChildWidget->Tooltip = Tooltip;

						ChildWidget->RefreshNode(SelectedChar, this);
						
					}
					else
					{
						ChildWidget->SetVisibility(ESlateVisibility::Hidden);
					}
				}
			}
		}
	}
}

void UBattleHudWidget::OnRoundEnds()
{
	for (int i = 0; i < SkillNodes.Num(); ++i)
	{
		if(SkillNodes[i])
			SkillNodes[i]->OnRoundEnds();
	}
}

void UBattleHudWidget::RefreshSkillButtonsState()
{
	for (int i = 0; i < SkillNodes.Num(); ++i)
	{
		if(SkillNodes[i])
		{
			SkillNodes[i]->RefreshNodeState();
		}
	}
}

void UBattleHudWidget::OnApChange()
{
	for (int i = 0; i < SkillNodes.Num(); ++i)
	{
		if(SkillNodes[i])
		{
			SkillNodes[i]->OnApChange();
		}
	}

	if(ApText && LatestSelectedChar)
	{
		ApText->SetText(FText::AsNumber(LatestSelectedChar->GetCurrentAP()));
	}
}

void UBattleHudWidget::SetSkillsPanelHidden()
{
	if(SkillsPanel)
	{
		SkillsPanel->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UBattleHudWidget::SetSkillsPanelVisible()
{
	if(SkillsPanel)
	{
		SkillsPanel->SetVisibility(ESlateVisibility::Visible);
		SkillsPanel->SetIsEnabled(true);
	}
}
