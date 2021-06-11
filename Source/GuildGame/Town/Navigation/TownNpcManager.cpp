// Fill out your copyright notice in the Description page of Project Settings.


#include "TownNpcManager.h"

#include "GuildGame/Characters/CharacterStats.h"
#include "GuildGame/Managers/CharacterManager.h"
#include "GuildGame/Town/TownGameModeBase.h"
#include "TownNavNodeActor.h"
#include "TownNpcCharacter.h"


class ATownNpcAIController;


TownNpcManager::TownNpcManager(ATownGameModeBase* GameMode)
{
    if(GameMode != nullptr)
    {
        TownGameMode = GameMode;
    }
}

void TownNpcManager::StartSpawning(TArray<FCharacterStats*> OwnedCharacters)
{
    for (int i = 0; i < OwnedCharacters.Num(); ++i)
    {
        SpawnNpcToRandomLocation( OwnedCharacters[i]->UniqueID,  OwnedCharacters[i]->TownNpcBehaviourState, OwnedCharacters[i]->ClassType);
    }
}

void TownNpcManager::SpawnOnClick()
{
    const int RandIndex = FMath::RandRange(0, NavigationNodes.Num() - 1);
    FVector RandLocation = GetRandomLocationFromNode(100);

    if(TownGameMode && TownGameMode->GetWorld())
    {
        ATownNpcCharacter* SpawnedCharacter = CharacterManager::SpawnCharacter<ATownNpcCharacter,ATownNpcCharacter>(TownGameMode->NpcCharacterBlueprint,ECharacterClassType::Knight,
            RandLocation, FRotator::ZeroRotator,TownGameMode->GetWorld());
        if(SpawnedCharacter)
        {
            MoveNpcToDestination(SpawnedCharacter);
        }
    }
}

void TownNpcManager::SpawnNpcToRandomLocation(int UniqueID, ENpcBehaviourStates State, ECharacterClassType CharacterType)
{
    const FVector RandLocation = GetRandomLocationFromNode(100);
    
    if(TownGameMode)
    {
        ATownNpcCharacter* SpawnedCharacter = CharacterManager::SpawnCharacter<ATownNpcCharacter,ATownNpcCharacter>(TownGameMode->NpcCharacterBlueprint,CharacterType,
            RandLocation, FRotator::ZeroRotator,TownGameMode->GetWorld());
        if(SpawnedCharacter)
        {
            SpawnedNpCs.Add(UniqueID, SpawnedCharacter);
            MoveNpcToDestination(SpawnedCharacter);
            SpawnedCharacter->SetBehaviourState(State);
        }
    }
}

void TownNpcManager::SetNpcBehaviourState(int UniqueID, ENpcBehaviourStates State, ECharacterClassType CharacterType)
{
    if(SpawnedNpCs.Contains(UniqueID))
    {
        //UE_LOG(LogTemp, Warning, TEXT("Spawn CONTAINS "));
        ATownNpcCharacter** Character =  SpawnedNpCs.Find(UniqueID);

        if(Character)
        {
            if(*Character)
            {
                (*Character)->SetBehaviourState(State);
            }
        }
    }
    else
    {
        SpawnNpcToRandomLocation( UniqueID,  State,  CharacterType);
    }
}

void TownNpcManager::MoveNpcToDestination(ATownNpcCharacter* SpawnedActor)
{
    ATownNavNodeActor* RandomNavNode =  GetRandomNavigationNode();
    SpawnedActor->DestinationNode = RandomNavNode;
    SpawnedActor->MoveToRandomLocation();
}

FString TownNpcManager::GetNpcTableRowName(ECharacterClassType CharacterType)
{
    if(CharacterType == ECharacterClassType::Knight)
    {
        return  FString(TEXT("Knight"));
    }
    else if(CharacterType == ECharacterClassType::Mage)
    {
        return  FString(TEXT("Mage"));
    }
    else if(CharacterType == ECharacterClassType::Archer)
    {
        return  FString(TEXT("Archer"));
    }
   
    return FString(TEXT("EMPTY"));
}

ATownNavNodeActor* TownNpcManager::GetRandomNavigationNode()
{
    if(NavigationNodes.Num() <= 0)
    {
        return nullptr;
    }
    
    const int RandIndex = FMath::RandRange(0, NavigationNodes.Num() - 1);
    FVector Location;
    return NavigationNodes[RandIndex];
}

FVector TownNpcManager::GetRandomLocationFromNode(float Zoffset)
{
    ATownNavNodeActor* RandomNavNode =  GetRandomNavigationNode();

    FVector Location;
    if(RandomNavNode)
    {
        Location = RandomNavNode->Destination;
		Location.Z += Zoffset;
    }

    return Location;
    
}
