// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GuildGame/Skills/CharacterSkill.h"

#include "CharacterSkillNodeWidget.generated.h"


UENUM()
enum class ESkillNodeState: uint8
{
	Locked,
	Unlocked,
	CanBeUnlocked
};

UCLASS()
class GUILDGAME_API UCharacterSkillNodeWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	UFUNCTION()
	void OnPressed();

	UPROPERTY(meta = (BindWidget))
	class UButton* SkillButton;

	bool bIsConstructorCalledOnce = false;
	FButtonStyle ButtonStyle;

public:

	ESkillNodeState NodeState;

	UPROPERTY(meta = (BindWidget))
	class UImage* Portrait;
	
	UPROPERTY(EditAnywhere)
	int UnlockSkillPointCost;
	
	UPROPERTY(EditAnywhere)
	TArray<int> PrerequisitesSkillIDs;

	UPROPERTY(EditAnywhere)
	int SkillID;

	FSkillData SkillsData;
	
	UPROPERTY()
	UTexture* SkillTexture;

	FText DescText;

	UPROPERTY(EditAnywhere)
	FSlateBrush CanBeUnlockedButtonBrush;
	
	bool bIsPressed;

	UPROPERTY()
	class UCharacterSkillsWidget* OwnerWidget;

	UPROPERTY()
	TArray<class UCharacterSkillLineWidget*> Lines;

	UPROPERTY()
	class UCharacterSkillTooltipWidget* Tooltip;

	void ReleaseButton();
	void ResetButtonStyle(bool Normal, bool Hover, bool Pressed);

	void SetSkillNodeState(ESkillNodeState State);


};
