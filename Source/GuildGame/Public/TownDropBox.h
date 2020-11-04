// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "TownScrollBoxItem.h"
#include "Blueprint/UserWidget.h"
#include "TownDropBox.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UTownDropBox : public UUserWidget
{
	GENERATED_BODY()
	EScrollBoxType ScrollType;
	
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	                  UDragDropOperation* InOperation) override;

	public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UScaleBox* ContentScaleBox;
};
