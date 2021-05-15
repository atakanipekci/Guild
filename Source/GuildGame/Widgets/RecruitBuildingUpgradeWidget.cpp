// Fill out your copyright notice in the Description page of Project Settings.


#include "RecruitBuildingUpgradeWidget.h"
#include "RecruitBuildingUpgTooltipWidget.h"
#include "BuildingUpgradeNode.h"
#include "GuildGame/Managers/WidgetManager.h"
#include "GuildGame/Town/BuildingStats.h"
#include "GuildGame/Town/TownGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void URecruitBuildingUpgradeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	ATownGameModeBase* GameMode = Cast<ATownGameModeBase>(UGameplayStatics::GetGameMode(this));
	if(GameMode)
	{
		TableRows = GameMode->GetBuildingUpgradeRowsByType(EBuildingTypes::Recruit);
		 UE_LOG(LogTemp, Warning, TEXT("ROW COUNT ON UPG %d"), TableRows.Num());
	}

	if(Upgrades.Num() > 0)
	{
		for (int i = 0; i < Upgrades.Num() && i + 1 < TableRows.Num(); ++i)
		{
			URecruitBuildingUpgTooltipWidget* Tooltip = CreateWidget<URecruitBuildingUpgTooltipWidget>(GetWorld(), WidgetManager::GetWidget(EWidgetKeys::RecruitBuildingUpgTooltip));
			if(Tooltip)
			{
				FRecruitBuildingUpgradeStats* PrevRow = static_cast<FRecruitBuildingUpgradeStats*>(TableRows[i]);
				FRecruitBuildingUpgradeStats* Row = static_cast<FRecruitBuildingUpgradeStats*>(TableRows[i + 1]);
				if(Row && PrevRow)
				{
					Tooltip->Refresh(Row, PrevRow, i + 1);
					Upgrades[i]->SetToolTip(Cast<UWidget>(Tooltip));
				}
			}
		}
	}

	
}
