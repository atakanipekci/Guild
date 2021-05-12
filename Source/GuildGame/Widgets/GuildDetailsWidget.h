// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GuildDetailsWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UGuildDetailsWidget : public UUserWidget
{
	GENERATED_BODY()
	
	public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GuildName;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GuildFameLevelText;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* GuildFameProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UButton* GainFameBtn;

	struct FGuildStats* GuildStats;
	void RefreshPage(struct FGuildStats* Stat);
	UFUNCTION()
	void GainFame();
	virtual void NativeConstruct() override;
};


