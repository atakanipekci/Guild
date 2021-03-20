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
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	int DraggedIndex = 0;

	public:
		void OnDragLeaveFromArea();

		
		int LatestChildIndex = 0;

		UPROPERTY()
		class UDroppableAreaWidget* OwnerDroppableArea;

		struct FCharacterStats* Stat;

		UPROPERTY(meta = (BindWidget))
		class UTextBlock* CharacterNameText;

		UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UUserWidget> DraggedVisualBp;

		UPROPERTY()
		UUserWidget* DraggedVisualInstance;

		UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UDragDropOperation> DragDropOperation;

		UFUNCTION(BlueprintCallable)
		void SetOwnerAreaWidget(UDroppableAreaWidget* Owner);

		void SetCharacterNameText(FText Text);
		void SetStat(FCharacterStats* NewStat);
};
