// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffectTooltipWidget.h"

#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/RichTextBlock.h"
#include "GuildGame/Managers/StatusEffectManager.h"

void UStatusEffectTooltipWidget::Refresh(const FStatusEffectFileDataTable& File, const FStatusEffectData& Data)
{
	if(StatusName)
		StatusName->SetText(File.StatusName);

	if(StatusImage)
		StatusImage->SetBrushResourceObject(File.Image);

	FFormatNamedArguments Args;
	Args.Add("MaxValue", Data.MaxValue);
	Args.Add("MinValue", Data.MinValue);
	Args.Add("RemainingTurns", Data.RemainingTurns);

	FText FormattedText = FText::Format(
		File.StatusDesc,
		Args
	);		
	DescText->SetText(FormattedText);

	if(RemainingOverlay && RemainingText)
	{
		if(Data.RemainingTurns > 0)
		{
			RemainingOverlay->SetVisibility(ESlateVisibility::Visible);
			Args.Add("Remaining", Data.RemainingTurns);

			const FText RemainingLocalizableText = NSLOCTEXT("CommonWords", "Remaining:", "Remaining: <Red>{Remaining}</> turn(s)");
			
			FormattedText = FText::Format(
				RemainingLocalizableText,
				Args
			);		
			RemainingText->SetText(FormattedText);
		}
		else
		{
			RemainingOverlay->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
