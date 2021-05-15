// Fill out your copyright notice in the Description page of Project Settings.


#include "GuildDetailsWidget.h"

#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GuildGame/GuildStats.h"

void UGuildDetailsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	 if(GainFameBtn)
        GainFameBtn->OnClicked.AddUniqueDynamic(this, &UGuildDetailsWidget::GainFame);
}

void UGuildDetailsWidget::RefreshPage(FGuildStats* Stat)
{
	GuildStats = Stat;

	if(Stat)
	{
		if(GuildName)
			GuildName->SetText(FText::FromString(Stat->GuildName));

		if(GuildFameLevelText)
			GuildFameLevelText->SetText(FText::FromString(Stat->FameLevelsDisplayNames[FMath::Clamp(FMath::Max(0, Stat->FameLevel - 1),0, Stat->FameLevelsDisplayNames.Num() - 1)]));

		if(GuildFameProgressBar)
			GuildFameProgressBar->SetPercent(Stat->GetLevelPercentage());
		
	}
}

void UGuildDetailsWidget::GainFame()
{
	GuildStats->GainFameExperience(1200);
	RefreshPage(GuildStats);
}


