// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ApNodeWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UApNodeWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UImage* FilledImage;
	UPROPERTY(meta = (BindWidget))
	class UImage* BlankImage;
public:
	void SetFull();
	void SetBlank();
};
