// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleHudWidget.h"

#include "BattleCharSkillNodeWidget.h"
#include "CharacterSkillTooltipWidget.h"
#include "GuildGameInstance.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
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
	
    if(SkillNodesGrid && SkillsPanel)
	{
    	SkillsPanel->SetVisibility(ESlateVisibility::Visible);
    	
        SkillNodes.Empty();
    	UGuildGameInstance* GameInstance = Cast<UGuildGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	    if(GameInstance == nullptr || GameInstance->CharacterSkillsFileTable == nullptr)
	    {
	        return;
	    }
    	
		for (int i = 0; i < SkillNodesGrid->GetChildrenCount(); ++i)
		{
			
			UBattleCharSkillNodeWidget* ChildWidget = Cast<UBattleCharSkillNodeWidget>(SkillNodesGrid->GetChildAt(i));
			if(ChildWidget && GameInstance)
			{
				TArray<class CharacterSkill*>* Skills = SelectedChar->GetSkills();
				
				
				if(Skills && i < Skills->Num())
				{
					CharacterSkill* Skill = (*Skills)[i];
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

							ChildWidget->RefreshNode(SelectedChar);
						}
					}
				}
				else
				{
					ChildWidget->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		}
	}
}
