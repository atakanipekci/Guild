// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DroppableAreaWidget.h"
#include "OwnedCharactersDroppableWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UOwnedCharactersDroppableWidget : public UDroppableAreaWidget
{
	GENERATED_BODY()

	public:
		virtual void NativeConstruct() override;
		virtual void DropFrom(UDraggedCharacterWidget* DraggedWidget) override;
		virtual bool DropTo(UDraggedCharacterWidget* DraggedWidget) override;
	    void Refresh() ;
		UDraggedCharacterWidget* CreateChildWidget(UDraggedCharacterWidget* DraggedWidget);
		UDraggedCharacterWidget* CreateChildWidget(struct FCharacterStats* Stat);

		UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UScrollBox* ScrollBox;

		UPROPERTY()
		class ATownGameModeBase* GameMode;
	
};
