// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildingUpgradeNode.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UBuildingUpgradeNode : public UUserWidget
{
	GENERATED_BODY()

	public:
	UPROPERTY(meta = (BindWidget))
	class UImage* UpgradeBg;
};
