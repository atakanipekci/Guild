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
	
	
};
