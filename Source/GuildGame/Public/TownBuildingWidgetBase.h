// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TownBuildingWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UTownBuildingWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	public:
	UPROPERTY(meta = (BindWidgetAnim), EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UWidgetAnimation* OpenUpAnimation;

	UPROPERTY(meta = (BindWidgetAnim), EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UWidgetAnimation* CloseDownAnimation;
};
