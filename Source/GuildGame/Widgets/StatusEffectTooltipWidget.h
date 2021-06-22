// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatusEffectTooltipWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UStatusEffectTooltipWidget : public UUserWidget
{
	GENERATED_BODY()

	public:
	
	void Refresh(const struct FStatusEffectFileDataTable& File, const struct FStatusEffectData& Data);

	UPROPERTY(meta = (BindWidget))
	class URichTextBlock* StatusName;

	UPROPERTY(meta = (BindWidget))
	class URichTextBlock* DescText;
	
	UPROPERTY(meta = (BindWidget))
	class URichTextBlock* RemainingText;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* RemainingOverlay;

	UPROPERTY(meta = (BindWidget))
	class UImage* StatusImage;
};
