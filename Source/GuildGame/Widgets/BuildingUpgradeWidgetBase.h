// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildingUpgradeWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UBuildingUpgradeWidgetBase : public UUserWidget
{
	GENERATED_BODY()

	void RefreshPage();
	
	public:
	virtual void NativeConstruct() override;
	void RefreshPage(struct FBuildingStatsBase* Stat);
	UFUNCTION()
	void UpgradeBuildingLevel();

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* UpgradesGridPanel;

	UPROPERTY(meta = (BindWidget))
	class UButton* UpgradeButton;

	UPROPERTY()
	TArray<class UBuildingUpgradeNode*> Upgrades;

	struct FBuildingStatsBase* BuildingStats;
};
