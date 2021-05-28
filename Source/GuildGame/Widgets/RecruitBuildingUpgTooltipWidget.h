// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RecruitBuildingUpgTooltipWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API URecruitBuildingUpgTooltipWidget : public UUserWidget
{
	GENERATED_BODY()

	public:
	virtual void NativeConstruct() override;
	void Refresh(struct FRecruitBuildingUpgradeStats& UpgStats, struct FRecruitBuildingUpgradeStats& PrevUpgStats, int UpgradeLevel);
	

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* UpgradeLevelText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* UpgradeDescText;
	

	
};
