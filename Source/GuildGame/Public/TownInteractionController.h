// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TownInteractionController.generated.h"


/**
 * 
 */
UENUM(BlueprintType)
enum class EInteractionStatee: uint8
{
	BuildingSelection,
	BuildingWidget,
	MenuWidget
};

UCLASS()
class GUILDGAME_API UTownInteractionController : public UObject
{
	GENERATED_BODY()
public:
	UTownInteractionController();
	~UTownInteractionController();
	void ManuelConstructor(EInteractionStatee NewState, class ATownPlayerController* NewPlayerController);
	void Tick();
	void LeftClickHandler();
	
	void RaycastUnderCursor();
	void PlaySequence();
	void PlaySequenceReverse();
	void SetRenderCustomDepth(UActorComponent* Building,  bool EnableDepth);
	void ZoomInToBuilding();
	void ZoomOutFromBuilding();
	void ChangeState(EInteractionStatee From, EInteractionStatee To);
	void OnStateChange(const EInteractionStatee State, const bool IsFrom);
	void ToPrevStateState();

	UFUNCTION(BlueprintCallable)
	void ToggleWidget();
	
	UFUNCTION()
	void OnWidgetAnimationFinish();

	UFUNCTION()
	void OnSequenceFinish();

	EInteractionStatee State;
	EInteractionStatee PrevState;
	ATownPlayerController* PlayerController;

	
	class ULevelSequence* SequenceAsset;

	class ALevelSequenceActor* SequenceActor;

	class ULevelSequencePlayer* SequencePlayer;

	class UTownBuildingActorComponent* SelectedBuilding;

	class UTownBuildingActorComponent* HoveredBuilding;
	
	bool bEnableInput = true;

	bool bIsSequenceReversed;
};
