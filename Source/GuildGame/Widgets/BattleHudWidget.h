// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleHudWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UBattleHudWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* SkillsPanel;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* SkillNodesGrid;

	UPROPERTY()
	TArray<class UBattleCharSkillNodeWidget*> SkillNodes;

	UPROPERTY()
	class UCharacterSkillTooltipWidget* Tooltip;
	

public:
	void RefreshSkillsArray(class AGGCharacter* SelectedChar);

	void OnTurnEnds();
	void RefreshSkillButtonsState();
};
