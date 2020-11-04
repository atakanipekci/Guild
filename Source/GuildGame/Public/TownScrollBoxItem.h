// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "Blueprint/UserWidget.h"
#include "TownScrollBoxItem.generated.h"

struct FCharacterStats;

UENUM(BlueprintType)
enum class EScrollBoxType: uint8
{
	None,
	Menu,
	Recruit,
	SquadDrop
};

UCLASS()
class GUILDGAME_API UTownScrollBoxItem : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;

	public:
		EScrollBoxType ScrollType;
		EScrollBoxType PrevParentType = EScrollBoxType::None;

		FCharacterStats* Stat;

		UPROPERTY()
		UPanelWidget* PrevParentWidget;

		UFUNCTION(BlueprintCallable)
		void SetScrollType(EScrollBoxType Type);
		UFUNCTION(BlueprintCallable)
		void SetPrevParentType(EScrollBoxType Type, UPanelWidget* PrevParent);
};
