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

	UPROPERTY(meta = (BindWidget))
	class UImage* CooldownImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CooldownText;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* CooldownPanel;

	UPROPERTY(EditAnywhere)
	FSlateBrush CooldownBrush;

	UPROPERTY(EditAnywhere)
	FSlateBrush PressedBrush;

	FSlateBrush NormalBrush;

	FButtonStyle ButtonStyle;

	FSkillData SkillsData;
	
	UPROPERTY(EditAnywhere)
	UTexture* SkillTexture;

	UPROPERTY(EditAnywhere)
	FText DescText;

	UPROPERTY()
	class UCharacterSkillTooltipWidget* Tooltip;

	UPROPERTY()
	AGGCharacter* SelectedCharacter;

	UPROPERTY()
	class ABattlePlayerController* PlayerController;

	FCooldownTimer* SkillCooldownData;

	UPROPERTY()
	class UBattleHudWidget* OwnerHud;

	UPROPERTY(EditAnywhere)
	bool bIsMovementButton;

	UFUNCTION()
	void OnPressed();
	UFUNCTION()
	void RefreshNodeState();

	void RefreshNode(AGGCharacter* SelectedChar, class UBattleHudWidget* Hud);

	UFUNCTION()
	void OnSkillCasted();

	void OnRoundEnds();
};
