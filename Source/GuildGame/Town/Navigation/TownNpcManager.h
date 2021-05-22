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
struct FWeaponFileData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* RightHandWeaponStaticMesh;

 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* LeftHandWeaponStaticMesh;

 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* RightHandWeaponSkeletalMesh;

 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* LeftHandWeaponSkeletalMesh;

 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector RelativeLocationLeftHandWeapon;
 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator RelativeRotationLeftHandWeapon;

 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector RelativeLocationRightHandWeapon;
 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator RelativeRotationRightHandWeapon;
};

 USTRUCT(BlueprintType)
struct FCharFileDataTable : public FTableRowBase
{
    GENERATED_BODY()
 	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharacterClassType CharacterType;
 	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
 	FWeaponFileData WeaponData;

 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimationBP;
};

class GUILDGAME_API TownNpcManager
{

	public:
	TownNpcManager(class ATownGameModeBase* GameMode);

	void StartSpawning(TArray<struct FCharacterStats*> OwnedCharacters);

	void SpawnOnClick();
	void SpawnNpcToRandomLocation(int UniqueID, ENpcBehaviourStates State, ECharacterClassType CharacterType);
	//class ATownNpcCharacter* SpawnCharacter(ECharacterClassType CharacterType, ENpcBehaviourStates State);
	FString GetNpcTableRowName(ECharacterClassType CharacterType);

	class ATownNavNodeActor* GetRandomNavigationNode();
	FVector GetRandomLocationFromNode(float Zoffset);
	void MoveNpcToDestination(class ATownNpcCharacter* SpawnedActor);

	void SetNpcBehaviourState(int UniqueID, const ENpcBehaviourStates State, ECharacterClassType CharacterType);

	ATownGameModeBase* TownGameMode;
	TArray<class ATownNavNodeActor*> NavigationNodes;
	TMap<int, class ATownNpcCharacter*> SpawnedNpCs;

	

};



