// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSkillsWidget.h"

#include "CharacterSkillLineWidget.h"
#include "CharacterSkillNodeWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "GuildGame/Characters/CharacterStats.h"

void UCharacterSkillsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(AcquireSkillButton)
	{
		AcquireSkillButton->OnClicked.AddUniqueDynamic(this, &UCharacterSkillsWidget::AcquireSkill);
		AcquireSkillButton->SetVisibility(ESlateVisibility::Hidden);
	}
	//RefreshSkillsArray();

}

void UCharacterSkillsWidget::AcquireSkill()
{
	if(SelectedSkillNode && CharacterStat && AcquireSkillButton)
	{
		if(CanSkillBeAcquired(SelectedSkillNode))
		{
			CharacterStat->SpendableSkillPoints -= SelectedSkillNode->UnlockSkillPointCost;
			CharacterStat->SkillIDs.Add(SelectedSkillNode->SkillID);
			SelectedSkillNode->SetSkillNodeState(ESkillNodeState::Unlocked);
			AcquireSkillButton->SetVisibility(ESlateVisibility::Hidden);

			RefreshSkillNodes();
		}
	}
}

void UCharacterSkillsWidget::RefreshSkillsArray()
{
    SelectedSkillNode = nullptr;
	if(AcquireSkillButton)
	{
		AcquireSkillButton->SetVisibility(ESlateVisibility::Hidden);
	}

	
    if(SkillsPanel)
	{

    	if(LinesPanel)
		{
			Lines.Empty();
			for (int i = 0; i < LinesPanel->GetChildrenCount(); ++i)
			{
				
				UCharacterSkillLineWidget* ChildWidget = Cast<UCharacterSkillLineWidget>(LinesPanel->GetChildAt(i));
				if(ChildWidget)
				{
					Lines.Add(ChildWidget);
				}
			}
		}
    	
        SkillNodes.Empty();
		for (int i = 0; i < SkillsPanel->GetChildrenCount(); ++i)
		{
			
			UCharacterSkillNodeWidget* ChildWidget = Cast<UCharacterSkillNodeWidget>(SkillsPanel->GetChildAt(i));
			if(ChildWidget)
			{
				ChildWidget->Lines.Empty();
				for (int j = 0; j < Lines.Num(); ++j)
				{
					if(Lines[j])
					{
						if(Lines[j]->OwnerSkillID == ChildWidget->SkillID)
						{
							ChildWidget->Lines.Add(Lines[j]);
						}
					}
				}
				SkillNodes.Add(ChildWidget);
				ChildWidget->OwnerWidget = this;
				ChildWidget->bIsPressed = false;
			}
		}

    	RefreshSkillNodes();
	}

	
}

void UCharacterSkillsWidget::RefreshSkillNodes()
{
	for (int i = 0; i < SkillNodes.Num(); ++i)
	{
		UCharacterSkillNodeWidget* ChildWidget = Cast<UCharacterSkillNodeWidget>(SkillNodes[i]);
		if(ChildWidget)
		{
			if(CharacterStat)
			{
				ChildWidget->ResetButtonStyle();
				if(CharacterStat->SkillIDs.Contains(ChildWidget->SkillID))
				{
					ChildWidget->SetSkillNodeState(ESkillNodeState::Unlocked);
				}
				else 
				{
					bool bMeetsConditions = true;
					for (int j = 0; j < ChildWidget->PrerequisitesSkillIDs.Num(); ++j)
					{
						if(CharacterStat->SkillIDs.Contains(ChildWidget->PrerequisitesSkillIDs[j]) == false)
						{
							bMeetsConditions = false;
							break;
						}
					}

					if(bMeetsConditions == true)
					{
						bMeetsConditions = CanSkillBeAcquired(ChildWidget);
					}

					if(bMeetsConditions == false)
					{
						ChildWidget->SetSkillNodeState(ESkillNodeState::Locked);
					}
					else
					{
						ChildWidget->SetSkillNodeState(ESkillNodeState::CanBeUnlocked);
					}
				}
			}
		}
	}
}

void UCharacterSkillsWidget::RefreshPage(FCharacterStats* Stat)
{
   if(Stat)
   {
       CharacterStat = Stat;
   	   RefreshSkillsArray();
   }
}

void UCharacterSkillsWidget::ReleaseSkillNodeButtons(UCharacterSkillNodeWidget* Excluded)
{
	if(Excluded && AcquireSkillButton)
	{
		if(Excluded->NodeState == ESkillNodeState::CanBeUnlocked)
		{
			AcquireSkillButton->SetVisibility(ESlateVisibility::Visible);
			AcquireSkillButton->SetIsEnabled(true);
		}
		else if(Excluded->NodeState == ESkillNodeState::Unlocked)
		{
			AcquireSkillButton->SetVisibility(ESlateVisibility::Hidden);
			AcquireSkillButton->SetIsEnabled(true);
		}
		else if(Excluded->NodeState == ESkillNodeState::Locked)
		{
			AcquireSkillButton->SetVisibility(ESlateVisibility::Visible);
			AcquireSkillButton->SetIsEnabled(false);
		}
	}
	for (int i = 0; i < SkillNodes.Num(); ++i)
	{
		UCharacterSkillNodeWidget* ChildWidget = SkillNodes[i];
		if(ChildWidget)
		{
			if(ChildWidget != Excluded)
			{
				ChildWidget->ReleaseButton();
			}
		}
	}
}

bool UCharacterSkillsWidget::CanSkillBeAcquired(UCharacterSkillNodeWidget* SkillNode)
{
	if(SkillNode && CharacterStat)
	{
		if(CharacterStat->SkillIDs.Contains(SkillNode->SkillID) == false)
		{
			if(CharacterStat->SpendableSkillPoints >= SkillNode->UnlockSkillPointCost)
			{
				return true;
			}
		}
	}
	
	return false;
}
