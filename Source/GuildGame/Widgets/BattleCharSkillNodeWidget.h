// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GuildGame/Skills/CharacterSkill.h"

#include "BattleCharSkillNodeWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UBattleCharSkillNodeWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	
	UPROPERTY(meta = (BindWidget))
	class UButton* SkillButton;

	UPROPERTY(meta = (BindWidget))
	class UImage* PortraitBg;

	// UPROPERTY(meta = (BindWidget))
	// class UImage* Portrait;

	UPROPERTY(EditAnywhere)
	FSlateColor CooldownButtonBrushColor;

	FButtonStyle ButtonStyle;

	FSkillData SkillsData;
	
	UPROPERTY()
	UTexture* SkillTexture;

	FText DescText;

	UPROPERTY()
	class UCharacterSkillTooltipWidget* Tooltip;

	UPROPERTY()
	AGGCharacter* SelectedCharacter;

	UPROPERTY()
	class ABattlePlayerController* PlayerController;

	UFUNCTION()
	void OnPressed();

	void RefreshNode(AGGCharacter* SelectedChar);
};
