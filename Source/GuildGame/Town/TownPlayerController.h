// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TownPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API ATownPlayerController : public APlayerController
{
	GENERATED_BODY()
	public:
		ATownPlayerController();
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaSeconds) override;
		virtual void SetupInputComponent() override;
		virtual void BeginDestroy() override;

		void LeftClickHandler();
	

		UFUNCTION(BlueprintCallable)
		void InteractionControllerWidgetToggle();

		UPROPERTY()
		class UTownBuildingInteractionManager* BuildingInteractionManager;
};
