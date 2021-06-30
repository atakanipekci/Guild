// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/UserWidgetPool.h"
#include "GuildGame/Managers/StatusEffectManager.h"

#include "StatusEffectNodeWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UStatusEffectNodeWidget : public UUserWidget
{
	GENERATED_BODY()

	

	TArray<const struct FStatusEffectData*> StatusEffectData;
	const struct FStatusEffectFileDataTable* StatusFile;


	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
public:
	void SetStatusEffectNode(const struct FStatusEffectData* Data, const struct FStatusEffectFileDataTable* File);
	void StackStatusEffect(struct FStatusEffectData* Data);
	
	UPROPERTY(meta = (BindWidget))
	class UImage* StatusImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TurnText;

	EStatusEffectType StatusType;

	UPROPERTY()
	class UStatusEffectStackableTooltipWidg* StatusTooltip;
	FUserWidgetPool* TooltipPool;

};
