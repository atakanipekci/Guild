// Fill out your copyright notice in the Description page of Project Settings.


#include "RecruitBuildingUpgTooltipWidget.h"

#include "Components/TextBlock.h"
#include "GuildGame/Town/BuildingStats.h"

void URecruitBuildingUpgTooltipWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void URecruitBuildingUpgTooltipWidget::Refresh(FRecruitBuildingUpgradeStats* UpgStats, FRecruitBuildingUpgradeStats* PrevUpgStats, int UpgradeLevel)
{
	RecruitBuildingUpgradeData = UpgStats;
	PrevRecruitBuildingUpgradeData = PrevUpgStats;
	
	if(RecruitBuildingUpgradeData && PrevRecruitBuildingUpgradeData)
	{
		if(UpgradeLevelText)
			UpgradeLevelText->SetText(FText::FromString(FString::Printf(TEXT("Level %d"), UpgradeLevel)));

		FString Descr;
		
		const int CharCount = RecruitBuildingUpgradeData->GenCharCount - PrevRecruitBuildingUpgradeData->GenCharCount;
		if(CharCount != 0)
		{
			Descr += "Character Limit +" + FString::FromInt(CharCount) + ", ";
		}

		const int LevelLimit = RecruitBuildingUpgradeData->GenCharHigherLevelLimit - PrevRecruitBuildingUpgradeData->GenCharHigherLevelLimit;
		if(LevelLimit != 0)
		{
			Descr += "Level Limit +" + FString::FromInt(LevelLimit) + ", ";
		}

		const int HigherLevelChance = RecruitBuildingUpgradeData->GenCharWithHigherLevelPercentage - PrevRecruitBuildingUpgradeData->GenCharWithHigherLevelPercentage;
		if(HigherLevelChance != 0)
		{
			Descr += "Higher Level Chance +%" + FString::FromInt(HigherLevelChance) + ", ";
		}

		const int WageDiscount = RecruitBuildingUpgradeData->GenCharWageDiscountPercentage - PrevRecruitBuildingUpgradeData->GenCharWageDiscountPercentage;
		if(WageDiscount != 0)
		{
			Descr += "Wage Discount +%" + FString::FromInt(WageDiscount) + ", ";
		}

		const int TraitPercentage = RecruitBuildingUpgradeData->GenCharWithTraitPercentage - PrevRecruitBuildingUpgradeData->GenCharWithTraitPercentage;
		if(TraitPercentage != 0)
		{
			Descr += "Trait Gain Chance +%" + FString::FromInt(TraitPercentage) + ", ";
		}

		const int ContractTraitPercentage = RecruitBuildingUpgradeData->GenCharWithContractTraitPercentage - PrevRecruitBuildingUpgradeData->GenCharWithContractTraitPercentage;
		if(ContractTraitPercentage != 0)
		{
			Descr += "Contract Trait Gain Chance +%" + FString::FromInt(ContractTraitPercentage) + ", ";
		}

		Descr.RemoveAt(FMath::Max(0, Descr.Len() - 2), 2);
		
		
		if(UpgradeDescText)
			UpgradeDescText->SetText(FText::FromString(Descr));
	}
}
