// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
	
	UFUNCTION()
	void OnPressed();

	UPROPERTY(meta = (BindWidget))
	class UButton* SkillButton;

	UPROPERTY(meta = (BindWidget))
	class UImage* Portrait;

	FButtonStyle ButtonStyle;

public:

	ESkillNodeState NodeState;
	
	UPROPERTY(EditAnywhere)
	int UnlockSkillPointCost;
	
	UPROPERTY(EditAnywhere)
	TArray<int> PrerequisitesSkillIDs;

	UPROPERTY(EditAnywhere)
	int SkillID;
	
	bool bIsPressed;

	UPROPERTY()
	class UCharacterSkillsWidget* OwnerWidget;

	UPROPERTY()
	TArray<class UCharacterSkillLineWidget*> Lines;

	void ReleaseButton();
	void ResetButtonStyle();

	void SetSkillNodeState(ESkillNodeState State);


};
