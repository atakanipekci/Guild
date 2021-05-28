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
	
	void Refresh(struct FSkillData&, UTexture* Image, FText* Desc);

	UPROPERTY(meta = (BindWidget))
	class URichTextBlock* SkillName;

	UPROPERTY(meta = (BindWidget))
	class URichTextBlock* DescText;

	UPROPERTY(meta = (BindWidget))
	class UImage* SkillImage;
};
