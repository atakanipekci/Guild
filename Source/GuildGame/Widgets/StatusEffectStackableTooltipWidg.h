// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatusEffectStackableTooltipWidg.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UStatusEffectStackableTooltipWidg : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* StatusNodeVerticalBox;
	
};
