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
		 //UE_LOG(LogTemp, Warning, TEXT("ROW COUNT ON UPG %d"), TableRows.Num());
	}

	if(Upgrades.Num() > 0)
	{
		Tooltip = CreateWidget<URecruitBuildingUpgTooltipWidget>(GetWorld(), AWidgetManager::GetWidget(EWidgetKeys::RecruitBuildingUpgTooltip, GetWorld()));
		
		for (int i = 0; i < Upgrades.Num() && i + 1 < TableRows.Num(); ++i)
		{
			if(Tooltip)
			{
				//RefreshTooltip(*Tooltip, i);
				Upgrades[i]->SetToolTip(Cast<UWidget>(Tooltip));
				Upgrades[i]->OwnerWidget = this;
				Upgrades[i]->Index = i;
			}
		}
	}
}

void URecruitBuildingUpgradeWidget::RefreshTooltip(int NodeIndex)
{
	if(Upgrades.Num() > 0)
	{
		if (NodeIndex < Upgrades.Num() && NodeIndex + 1 < TableRows.Num())
		{
			FRecruitBuildingUpgradeStats* PrevRow = static_cast<FRecruitBuildingUpgradeStats*>(TableRows[NodeIndex]);
			FRecruitBuildingUpgradeStats* Row = static_cast<FRecruitBuildingUpgradeStats*>(TableRows[NodeIndex + 1]);
			if(Row && PrevRow && Tooltip)
			{
				Tooltip->Refresh(*Row, *PrevRow, NodeIndex + 1);
			}
		}
	}
}
