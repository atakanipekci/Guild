// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSkillsWidget.h"

#include "CharacterSkillNodeWidget.h"
#include "Components/CanvasPanel.h"

void UCharacterSkillsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RefreshSkillsArray();

}
void UCharacterSkillsWidget::RefreshSkillsArray()
{
    if(SkillsPanel)
	{
        SkillNodes.Empty();
		for (int i = 0; i < SkillsPanel->GetChildrenCount(); ++i)
		{
			UCharacterSkillNodeWidget* ChildWidget = Cast<UCharacterSkillNodeWidget>(SkillsPanel->GetChildAt(i));
			if(ChildWidget)
			{
				SkillNodes.Add(ChildWidget);
				ChildWidget->OwnerWidget = this;
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
