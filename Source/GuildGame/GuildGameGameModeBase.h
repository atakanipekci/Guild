// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TSubclassOf<class AGGCharacter> BattleCharactersBP;
	const TArray<class AGGCharacter*>& GetCharacterList() const
	{
		return Characters;
	}
private:
	TArray<class AGGCharacter*> Characters;
};
