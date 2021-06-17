// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSkillTooltipWidget.h"

#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/RichTextBlock.h"
#include "GuildGame/Skills/CharacterSkill.h"

void UCharacterSkillTooltipWidget::Refresh(FSkillData& Data, UTexture* Image, FText* Desc)
{
	if(SkillName)
		SkillName->SetText(Data.SkillName);

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
	
	if(CooldownOverlay && CooldownText)
	{
		if(Data.Cooldown > 0)
		{
			CooldownOverlay->SetVisibility(ESlateVisibility::Visible);
			FFormatNamedArguments Args;
			Args.Add("Cooldown", Data.Cooldown);

			const FText CooldownLocalizableText = NSLOCTEXT("CommonWords", "Cooldown:", "Cooldown: <Red>{Cooldown}</> turn(s)");
			
			const FText FormattedText = FText::Format(
				CooldownLocalizableText,
				Args
			);		
			CooldownText->SetText(FormattedText);
		}
		else
		{
			CooldownOverlay->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if(ApCostOverlay && ApCostText)
	{
		if(Data.Cooldown > 0)
		{
			ApCostOverlay->SetVisibility(ESlateVisibility::Visible);
			FFormatNamedArguments Args;
			Args.Add("ApCost", Data.ApCost);

			const FText ApCostLocalizableText = NSLOCTEXT("CommonWords", "ApCost:", "Ap Cost: <Red>{ApCost}</>");
			
			const FText FormattedText = FText::Format(
				ApCostLocalizableText,
				Args
			);		
			ApCostText->SetText(FormattedText);
		}
		else
		{
			ApCostOverlay->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

