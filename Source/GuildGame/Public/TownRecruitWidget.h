// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TownBuildingWidgetBase.h"

#include "TownRecruitWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UTownRecruitWidget : public UTownBuildingWidgetBase
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	virtual void Refresh() override;
	
	
};
