// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/GameInstance.h"
#include "TownGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UTownGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	int Gold;
	TArray<struct FCharacterStats*> SquadCharacters;
	
	UFUNCTION()
	bool TryToPurchase(int Amount);
};
