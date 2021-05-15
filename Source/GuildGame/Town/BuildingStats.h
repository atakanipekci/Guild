#pragma once
#include "Engine/DataTable.h"
#include "BuildingStats.generated.h"

UENUM()
enum class EBuildingTypes: uint8
{
	Recruit,
	Tavern,
	Cemetery
};

USTRUCT()
struct FBuildingStatsBase
{
	GENERATED_BODY()
	EBuildingTypes BuildingType;
	int UpgradeLevel = 1;
};

USTRUCT()
struct FRecruitBuildingUpgradeStats : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int GenCharCount = 4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int GenCharWithHigherLevelPercentage = 0;//all characters are level 1,
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int GenCharHigherLevelLimit = 2;//Level 2 is the first limit
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int GenCharWageDiscountPercentage = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int GenCharWithTraitPercentage = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int GenCharWithContractTraitPercentage = 0;

	FRecruitBuildingUpgradeStats()
	{
		
	}

	explicit FRecruitBuildingUpgradeStats(const FRecruitBuildingUpgradeStats& StructForCopy)
	{
		GenCharCount = StructForCopy.GenCharCount;
		GenCharWithHigherLevelPercentage = StructForCopy.GenCharWithHigherLevelPercentage;
		GenCharHigherLevelLimit = StructForCopy.GenCharHigherLevelLimit;
		GenCharWageDiscountPercentage = StructForCopy.GenCharWageDiscountPercentage;
		GenCharWithTraitPercentage = StructForCopy.GenCharWithTraitPercentage;
		GenCharWithContractTraitPercentage = StructForCopy.GenCharWithContractTraitPercentage;
	}
};

USTRUCT()
struct FRecruitBuildingStats : public FBuildingStatsBase
{
	GENERATED_BODY()

	FRecruitBuildingUpgradeStats* UpgradeStats;

	FRecruitBuildingStats()
	{
		UpgradeStats = new FRecruitBuildingUpgradeStats();
		BuildingType = EBuildingTypes::Recruit;
	}
	
};




