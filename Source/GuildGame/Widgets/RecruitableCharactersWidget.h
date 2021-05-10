// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DroppableAreaWidget.h"
#include "RecruitableCharactersWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API URecruitableCharactersWidget : public UDroppableAreaWidget
{
	GENERATED_BODY()
	public:
		virtual void NativeConstruct() override;
		virtual void DropFrom(UDraggedCharacterWidget* DraggedWidget) override;
		virtual bool DropTo(UDraggedCharacterWidget* DraggedWidget) override;

	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UScrollBox* ScrollBox;
};
