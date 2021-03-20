// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterDetailWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UCharacterDetailWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CharacterNameText;
};
