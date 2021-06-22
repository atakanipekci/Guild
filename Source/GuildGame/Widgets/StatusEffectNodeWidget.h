// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GuildGame/Managers/StatusEffectManager.h"

#include "StatusEffectNodeWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UStatusEffectNodeWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	class UStatusEffectStackableTooltipWidg* StatusTooltip;

	TArray<const struct FStatusEffectData*> StatusEffectData;
	const struct FStatusEffectFileDataTable* StatusFile;


	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
public:
	void SetStatusEffectNode(struct FStatusEffectData* Data, struct FStatusEffectFileDataTable* File, class UStatusEffectStackableTooltipWidg* Tooltip, TArray<class UStatusEffectTooltipWidget*>* ChildInstances);
	void StackStatusEffect(struct FStatusEffectData* Data);
	
	UPROPERTY(meta = (BindWidget))
	class UImage* StatusImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TurnText;

	EStatusEffectType StatusType;

	TArray<class UStatusEffectTooltipWidget*>* TooltipChildInstances;

};
