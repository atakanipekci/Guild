// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSkillTooltipWidget.h"

#include "Components/TextBlock.h"
#include "GuildGame/Skills/CharacterSkills.h"

void UCharacterSkillTooltipWidget::Refresh(FSkillData& Data)
{
	if(SkillName)
		SkillName->SetText(FText::FromString(Data.SkillName));

	if(DescText)
		DescText->SetText(FText::FromString("This Skill is very nice However I believe this is a very long ass Description to use on this skill my master"));
}
