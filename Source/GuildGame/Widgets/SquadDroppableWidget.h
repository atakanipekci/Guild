// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DroppableAreaWidget.h"
#include "SquadDroppableWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API USquadDroppableWidget : public UDroppableAreaWidget
{
	GENERATED_BODY()
	public:
		virtual void NativeConstruct() override;
		virtual void DropFrom(UDraggedCharacterWidget* DraggedWidget) override;
		virtual bool DropTo(UDraggedCharacterWidget* DraggedWidget) override;
		virtual UDraggedCharacterWidget* CreateAndAddChildToContentPanel(FCharacterStats* Stat, EWidgetKeys WidgetKey) override;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UScaleBox* ContentScaleBox;
};
