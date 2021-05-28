// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingUpgradeNode.h"

#include "RecruitBuildingUpgradeWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GuildGame/Town/BuildingStats.h"


void UBuildingUpgradeNode::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(OwnerWidget)
	{
		if(OwnerWidget->BuildingStats)
		{
			if(OwnerWidget->BuildingStats->BuildingType == EBuildingTypes::Recruit)
			{
				URecruitBuildingUpgradeWidget* Recruit = static_cast<URecruitBuildingUpgradeWidget*>(OwnerWidget);
				
				if(Recruit)
				{
					Recruit->RefreshTooltip(Index);
				}
			}
		}
	}
}
