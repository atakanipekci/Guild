// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GuildGameInstance.h"
#include "GuildGame/Town/Navigation/TownNpcManager.h"
#include "Kismet/GameplayStatics.h"

enum class ECharacterClassType: uint8;

/**
 * 
 */
class GUILDGAME_API CharacterManager
{
public:
    static void SetMovableGrids(class AGGCharacter* Character);
    static void SetDamageableGrids(class AGGCharacter* Character);
    static bool CanAttackTo(const class AGGCharacter*, const class AGGCharacter* );
    static class GridManager* CharGridManager;

	template<typename T, typename E>
	static T* SpawnCharacter(TSubclassOf<E> BPClass, ECharacterClassType CharacterType, FVector Location, FRotator Rotation, UWorld* World);

	static FString GetCharFileRowName(ECharacterClassType CharacterType);

	
};

template <typename T, typename E>
T* CharacterManager::SpawnCharacter(TSubclassOf<E> BPClass, ECharacterClassType CharacterType, FVector Location,
	FRotator Rotation, UWorld* World)
{
	if(World == nullptr)
    {
        return nullptr;
    }
    UGuildGameInstance* GameInstance = Cast<UGuildGameInstance>(UGameplayStatics::GetGameInstance(World));

    if(GameInstance == nullptr || GameInstance->CharacterFileTable == nullptr)
    {
        return nullptr;
    }
    T* SpawnedCharacter =  World->SpawnActor<T>(BPClass, Location, Rotation);
    if(SpawnedCharacter)
    {
        USkeletalMeshComponent* SkeletalMesh = Cast<USkeletalMeshComponent>(SpawnedCharacter->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
            
        if(SkeletalMesh)
        {
            const FName RowName = *(GetCharFileRowName(CharacterType));
            FCharFileDataTable* CharFiles = GameInstance->CharacterFileTable->FindRow<FCharFileDataTable>(RowName, "Character File Row Missing", true);

            if(CharFiles)
            {
                if(CharFiles->SkeletalMesh)
                {
                    SkeletalMesh->SetSkeletalMesh(CharFiles->SkeletalMesh);
        
                    if(CharFiles->AnimationBP)
                    {
                        SkeletalMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
                        SkeletalMesh->SetAnimInstanceClass(CharFiles->AnimationBP);
                    }
                }
            }       
        }
    }
    return SpawnedCharacter;
}
