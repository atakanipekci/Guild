// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSkillTooltipWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UCharacterSkillTooltipWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	void Refresh(struct FSkillData&);

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SkillName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DescText;
};
