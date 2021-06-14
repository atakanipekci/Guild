// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Managers/TurnManager.h"

#include "GuildGameGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API AGuildGameGameModeBase : public AGameModeBase
{
public:
	GENERATED_BODY()
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void Start();

	UFUNCTION(BlueprintCallable)
	void Next();

	void OnRoundEnds();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TSubclassOf<class AGGCharacter> BattleCharactersBP;
	const TArray<class AGGCharacter*>& GetCharacterList() const
	{
		return Characters;
	}
	TurnManager BattleTurnManager;

	UPROPERTY()
	class UBattleHudWidget* HudWidgetInstance;
	
private:
	TArray<class AGGCharacter*> Characters;
};
