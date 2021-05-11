// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BattleAIController.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API ABattleAIController : public AAIController
{
	GENERATED_BODY()

	void OnMoveCompleted(FAIRequestID, const FPathFollowingResult&) override;
};
