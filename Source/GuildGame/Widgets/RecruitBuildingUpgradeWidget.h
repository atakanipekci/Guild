// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingUpgradeWidgetBase.h"
#include "RecruitBuildingUpgradeWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API URecruitBuildingUpgradeWidget : public UBuildingUpgradeWidgetBase
{
	GENERATED_BODY()

	public:
	virtual void NativeConstruct() override;

	 TArray<struct FTableRowBase*> TableRows;
};
