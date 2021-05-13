// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "GuildGame/Characters/CharacterStats.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "TownNpcManager.generated.h"

enum class ECharacterClassType: uint8;
/**
 * 
 */
 USTRUCT(BlueprintType)
struct FCharFileDataTable : public FTableRowBase
{
    GENERATED_BODY()
 	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharacterClassType CharacterType;
 	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* SkeletalMesh;

 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimationBP;
};

UCLASS()
class GUILDGAME_API UTownNpcManager : public UObject
{
	GENERATED_BODY()


	public:
	void ManuelConstructor(/*class UDataTable* Table, */class ATownGameModeBase* GameMode);

	void StartSpawning(TArray<struct FCharacterStats*> OwnedCharacters);

	void SpawnOnClick();
	//class ATownNpcCharacter* SpawnCharacter(ECharacterClassType CharacterType, ENpcBehaviourStates State);
	FString GetNpcTableRowName(ECharacterClassType CharacterType);

	class ATownNavNodeActor* GetRandomNavigationNode();
	FVector GetRandomLocationFromNode(float Zoffset);
	void MoveNpcToDestination(class ATownNpcCharacter* SpawnedActor);

	virtual void BeginDestroy() override;
	
	void SetNpcBehaviourState(int UniqueID, const ENpcBehaviourStates State, ECharacterClassType CharacterType);

	UPROPERTY()
	UDataTable* NpcTable;
	UPROPERTY()
	ATownGameModeBase* TownGameMode;
	UPROPERTY()
	TArray<class ATownNavNodeActor*> NavigationNodes;
	UPROPERTY()
	TMap<int, class ATownNpcCharacter*> SpawnedNpCs;

	

};



