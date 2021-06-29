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
	Args.Add("Value", Data.Value);
	Args.Add("RemainingTurns", Data.RemainingTurns);

	FText FormattedText = FText::Format(
		File.StatusDesc,
		Args
	);
	
	if(Data.StatsMap.Num() > 0 && File.StatFiles.Num() > 0)
	{
		FFormatNamedArguments StatsArgs;
		TArray<FText> TextsToAdd;

		for (auto It = Data.StatsMap.CreateConstIterator(); It; ++It)
		{
			if(File.StatFiles.Contains(It.Key()))
			{
				const FStatusEffectStatsFileData* StatFile = File.StatFiles.Find(It.Key());
				if(It.Key() == EStatusEffectStatsType::Speed)
				{
					StatsArgs.Add("Speed", It.Value().Value);
				}

				if(StatFile)
				{
					FText StatFormattedText = FText::Format(
						StatFile->StatDesc,
						StatsArgs
					);

					TextsToAdd.Add(StatFormattedText);
				}
			}
		}
		
		if(TextsToAdd.Num() > 0)
		{
			FText StatsJoined = FText::Join(
				FText::FromString(", "),
				TextsToAdd
			);
			TextsToAdd.Empty();
			TextsToAdd.Add(FormattedText);
			TextsToAdd.Add(StatsJoined);
			FormattedText = FormattedText.Join(FText::GetEmpty(), TextsToAdd);
		}
	}
	
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
