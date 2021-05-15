// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingWidgetBase.h"

#include "RecruitWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API URecruitWidget : public UBuildingWidgetBase
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	virtual void Refresh() override;
	
	public:
	UFUNCTION()
	void RefreshRecruitables();

	UFUNCTION()
	void OpenUpgradePage();
	
	UPROPERTY(meta = (BindWidget))
	UButton* RefreshButton;
	UPROPERTY(meta = (BindWidget))
	UButton* UpgradeButton;
	UPROPERTY(meta = (BindWidget))
	class URecruitableCharactersDroppableWidget* RecruitableCharactersSlider;
};
