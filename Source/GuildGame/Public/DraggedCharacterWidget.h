// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "DraggedCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UDraggedCharacterWidget : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	public:
		class UDroppableAreaWidget* OwnerDroppableArea;
		struct FCharacterStats* Stat;

		UFUNCTION(BlueprintCallable)
		void SetOwnerAreaWidget(UDroppableAreaWidget* Owner);
};
