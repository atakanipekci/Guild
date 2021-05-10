// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GGAIController.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API AGGBasicAIController : public AAIController
{
	GENERATED_BODY()

	void OnMoveCompleted(FAIRequestID, const FPathFollowingResult&) override;
};
