// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/UserWidgetPool.h"

#include "TurnInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UTurnInfoWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* TurnScrollBox;

	UPROPERTY(meta = (BindWidget))
	class UGridPanel* TurnGridPanel;

	UPROPERTY(meta = (BindWidget))
	class UBorder* TurnBorder;
	TArray<class AGGCharacter*> CharactersList;

	int GridIndexHead = 0;
	int ActiveNodeIndex = 0;
	int RoundCount = 0;
public:
	float DefaultYSize = 100;
	float DefaultXSize = 100;
	float BigYSize = 120;

	TArray<TArray<class UTurnInfoNodeWidget*>> TurnNodes;

	// TQueue<class UTurnInfoNodeWidget*> Pool;
	FUserWidgetPool Pool;
	
	UTurnInfoNodeWidget* AddNode(bool IsRoundSeperator, class AGGCharacter* = nullptr);	
	UTurnInfoNodeWidget* CreateEmptyNode();	
	void FillGapWithRounds();
	void AddNewRound();
	void RemoveRandomNode(bool Slide);
	void RemoveAtTop(bool Slide);
	void RemoveNodeWidget(class UTurnInfoNodeWidget* Node ,bool Slide, bool IsFirst = false);
	void RemoveNodeFromList(int i, int j);
	void RemoveNodeFromList(class UTurnInfoNodeWidget* Node);
	void RefreshIndices();
	void RecursiveSlideAll(class UTurnInfoNodeWidget* Node, float PreviousWidth, float OffsetWidth, bool IsFirst, int ExpandingNodeGridIndex = 0, float ExpandingNodeExpandSize = 0);
	void SlideScrollbar();
	void Start(TArray<class AGGCharacter*> NewCharactersList);
	void UpdateOrder();
	void RemoveDeadCharacter(class AGGCharacter* CharToRemove);
	void SetCharactersList(TArray<class AGGCharacter*>& NewList);
};
