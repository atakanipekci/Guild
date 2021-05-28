// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSkillTooltipWidget.h"

#include "Components/Image.h"
#include "Components/RichTextBlock.h"
#include "GuildGame/Skills/CharacterSkill.h"

void UCharacterSkillTooltipWidget::Refresh(FSkillData& Data, UTexture* Image, FText* Desc)
{
	if(SkillName)
		SkillName->SetText(FText::FromString(Data.SkillName));

	if(SkillImage)
		SkillImage->SetBrushResourceObject(Image);

	if(DescText && Desc)
	{
		FFormatNamedArguments Args;
		Args.Add("Range", Data.Range);

		for (int i = 0; i < Data.EffectData.Num(); ++i)
		{
			FString Chance = "Chance";
			Chance.AppendInt(i);
			FString MaxDamage = "MaxDamage";
			MaxDamage.AppendInt(i);
			FString MinDamage = "MinDamage";
			MinDamage.AppendInt(i);
			
			Args.Add(Chance, Data.EffectData[i].Chance);
			Args.Add(MaxDamage, Data.EffectData[i].MaxValue);
			Args.Add(MinDamage, Data.EffectData[i].MinValue);
		}
		
		const FText FormattedText = FText::Format(
			*Desc,
			Args
		);		
		DescText->SetText(FormattedText);
	}
}
