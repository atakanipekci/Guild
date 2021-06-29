// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "TurnInfoNodeWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UTurnInfoNodeWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(meta = (BindWidget))
	class UImage* Portrait;
	UPROPERTY(meta = (BindWidget))
	class UImage* BackGround;
	UPROPERTY(meta = (BindWidget))
	class UImage* SeperatorBg;
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* NodePanel;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LeftText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* RightText;

	UPROPERTY(Transient , meta = (BindWidgetAnim))
	class UWidgetAnimation* AddAnimation;
	UPROPERTY(Transient , meta = (BindWidgetAnim))
	class UWidgetAnimation* RemoveAnimation;
	
	bool IsRemoved = false;
	bool IsSeperator = false;
	int Index;
	int GridIndex;

	int IndexFirst;
	int IndexSecond;
	float SwapOffSet = 0;
	
	UPROPERTY()
	class UTurnInfoNodeWidget* LeftNode;
	UPROPERTY()
	class UTurnInfoNodeWidget* RightNode;
	UPROPERTY()
	class AGGCharacter* Character;

	void EmptyData();
};
