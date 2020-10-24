// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TownYesOrNoWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE_RetVal(bool, FYesNoDelegate);
/**
 * 
 */
UCLASS()
class GUILDGAME_API UTownYesOrNoWidget : public UUserWidget
{
	GENERATED_BODY()

	public:
	
	UPROPERTY(meta = (BindWidget))
	class UButton* YesButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* NoButton;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TitleTextBlock;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ContentTextBlock;

	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnYesButtonClicked();
	
	UFUNCTION()
	void OnNoButtonClicked();

	FYesNoDelegate YesEvent;
	FYesNoDelegate NoEvent;
	
	static void CreateYesNoWidget(UUserWidget* Owner, UTownYesOrNoWidget* Widget , FText& Title, FText& Content, FYesNoDelegate YesDelegate, FYesNoDelegate NoDelegate);
};
