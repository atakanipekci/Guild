// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/GameInstance.h"
#include "GuildGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API UGuildGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	int Gold;

	int Day;

	TArray<struct FCharacterStats*> SquadCharacters;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	class UDataTable* WidgetsTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	class UDataTable* SequenceTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	UDataTable* CharactersTable;
	
	UFUNCTION()
	bool TryToPurchase(int Amount);

	UFUNCTION(BlueprintCallable)
	void ReturnToTown();

	void IncreaseDay(int Amount);

	virtual void Init() override;
	
};
