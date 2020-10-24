// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include <list>

#include "Blueprint/UserWidget.h"
#include "TownBuildingWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UTownBuildingWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	void virtual OnEnabled();
	
	UPROPERTY(meta = (BindWidgetAnim), EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UWidgetAnimation* OpenUpAnimation;

	UPROPERTY(meta = (BindWidgetAnim), EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UWidgetAnimation* CloseDownAnimation;

	

	//UPROPERTY(meta = (BindWidget))
	//class UBorder* ConfirmBorder;

	UFUNCTION(BlueprintCallable)
	void OnExitClicked();
	
	UFUNCTION()
	void OnConstructClicked();
	
	UFUNCTION()
	bool ConstrNoEvent();

	UFUNCTION()
	bool ConstrYesEvent();

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* MainCanvasPanel;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* PreviewCanvasPanel;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* UnderConstructionCanvasPanel;

	UPROPERTY(meta = (BindWidget))
	class UButton* ConstructButton;

	/*UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;*/

	/*UPROPERTY(meta = (BindWidget))
	class UBorder* UnderConstructionBorder;*/
	
};
