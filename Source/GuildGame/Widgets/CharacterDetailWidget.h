// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GuildGame/Managers/CharacterManager.h"

#include "CharacterDetailWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UCharacterDetailWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CharacterNameText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CharacterClassText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HealthText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BaseDamageText;
	UPROPERTY(meta = (BindWidget))
	class UImage* Portrait;



	UPROPERTY(meta = (BindWidget))
	class UButton* SkillsButton;

	struct FCharacterStats* CharacterStat;
	
	void RefreshPage(struct FCharacterStats* Stat);

	

	UFUNCTION()
	void OpenSkillsWidget();
};




