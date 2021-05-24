// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSkillLineWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UCharacterSkillLineWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UImage* LineImage;
public:
	UPROPERTY(EditAnywhere)
	int OwnerSkillID;

	void SetLineColorAndOpacity(FLinearColor Col);
};
