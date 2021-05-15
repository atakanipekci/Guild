// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingUpgradeWidgetBase.h"

#include "BuildingUpgradeNode.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/UniformGridPanel.h"
#include "GuildGame/Town/BuildingStats.h"
#include "GuildGame/Town/TownGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UBuildingUpgradeWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
	if(UpgradesGridPanel && Upgrades.Num() <= 0)
	{
		for (int i = 0; i < UpgradesGridPanel->GetChildrenCount(); ++i)
		{
			UBuildingUpgradeNode* ChildWidget = Cast<UBuildingUpgradeNode>(UpgradesGridPanel->GetChildAt(i));
			if(ChildWidget)
			{
				Upgrades.Add(ChildWidget);
			}
		}
	}

	if(UpgradeButton)
		UpgradeButton->OnClicked.AddUniqueDynamic(this, &UBuildingUpgradeWidgetBase::UpgradeBuildingLevel);
}

void UBuildingUpgradeWidgetBase::RefreshPage()
{
	if(UpgradesGridPanel && BuildingStats)
	{
		for (int i = 0; i < Upgrades.Num(); ++i)
		{
			if(Upgrades[i])
			{
				if(Upgrades[i]->UpgradeBg)
					Upgrades[i]->UpgradeBg->SetVisibility(ESlateVisibility::Collapsed);

				if(BuildingStats->UpgradeLevel > i + 1)
				{
					Upgrades[i]->SetIsEnabled(true);
				}
				else
				{
					Upgrades[i]->SetIsEnabled(false);
					if(BuildingStats->UpgradeLevel == i + 1)
					{
						if(Upgrades[i]->UpgradeBg)
							Upgrades[i]->UpgradeBg->SetVisibility(ESlateVisibility::Visible);
					}
				}
			}
		}
	}
}

void UBuildingUpgradeWidgetBase::RefreshPage(FBuildingStatsBase* Stat)
{
	if(Stat)
	{
		BuildingStats = Stat; 
	}
	RefreshPage();
}

void UBuildingUpgradeWidgetBase::UpgradeBuildingLevel()
{
	ATownGameModeBase* GameMode = Cast<ATownGameModeBase>(UGameplayStatics::GetGameMode(this));
	if(GameMode)
	{
		bool IsUpgradeAvailable = false;
		GameMode->UpgradeBuilding(BuildingStats->BuildingType, IsUpgradeAvailable);
		RefreshPage();

		if(IsUpgradeAvailable == false)
		{
			if(UpgradeButton)
				UpgradeButton->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
