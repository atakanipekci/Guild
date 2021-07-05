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

	UPROPERTY(meta = (BindWidget))
	class UButton* FinishTurnButton;

	UPROPERTY()
	TArray<class UBattleCharSkillNodeWidget*> SkillNodes;

	UPROPERTY()
	class UCharacterSkillTooltipWidget* Tooltip;

	UPROPERTY()
	class AGGCharacter* LatestSelectedChar;

	UPROPERTY(meta = (BindWidget))
	class UTurnInfoWidget* TurnWidget;

	UPROPERTY(meta = (BindWidget))
	class UApBarWidget* ApBar;
	

public:
	void RefreshSkillsArray(class AGGCharacter* SelectedChar);

	void OnRoundEnds();

	UFUNCTION()
	void RefreshSkillButtonsState();

	UFUNCTION()
	void RefreshHud();
	
	UFUNCTION()
	void FinishTurnClicked();

	UFUNCTION()
	void SetSkillsPanelHidden();

	UFUNCTION()
	void SetSkillsPanelVisible();

	void ResetApBar(int MaxAp);
	void RefreshApBar(int CurrentAp);
	void PreviewSpending(int ApToSpend);
	void StopPreview();

	UFUNCTION(BlueprintCallable)
	void RemoveRandomNode();

	UFUNCTION(BlueprintCallable)
	void RefreshIndices();

	class UTurnInfoWidget* GetTurnInfoWidget();
};
