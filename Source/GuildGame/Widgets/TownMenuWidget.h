// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "TownMenuWidget.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EMenuWidgetType : uint8
{
	WarCanvas,
	TeamCanvas
};
UCLASS()
class GUILDGAME_API UTownMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	protected:


	class ATownPlayerController* PlayerController;

	UPROPERTY(meta = (BindWidget))
	class UButton* TestButton;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TestTextBlock;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* GoldText;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UCanvasPanel* WarCanvas;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* TeamCanvas;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnTestClicked();

	UFUNCTION(BlueprintCallable)
	void ExitCanvas(EMenuWidgetType CanvasType);

	UFUNCTION(BlueprintCallable)
	void EnterCanvas(EMenuWidgetType CanvasType);

	public:
	UFUNCTION()
	void UpdateUI();
};