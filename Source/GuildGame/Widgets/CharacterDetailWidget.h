// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GuildGame/Characters/CharacterStats.h"

#include "CharacterDetailWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UCharacterDetailWidget : public UUserWidget
{
	GENERATED_BODY()

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
	void RefreshPage(FCharacterStats* Stat);
};
