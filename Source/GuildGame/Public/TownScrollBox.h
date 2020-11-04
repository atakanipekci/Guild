// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "TownScrollBoxItem.h"
#include "Blueprint/UserWidget.h"

#include "TownScrollBox.generated.h"

/**
 * 
 */


UCLASS()
class GUILDGAME_API UTownScrollBox : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;

	EScrollBoxType ScrollType;

	public:
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UScrollBox* ScrollBox;
	
	UFUNCTION(BlueprintCallable)
	void SetScrollType(EScrollBoxType Type);
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	                  UDragDropOperation* InOperation) override;
};
