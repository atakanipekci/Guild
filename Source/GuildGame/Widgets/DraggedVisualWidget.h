// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DraggedVisualWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UDraggedVisualWidget : public UUserWidget
{
	GENERATED_BODY()
	public:

	UPROPERTY(meta = (BindWidget))
	class UImage* Portrait;
};
