// Fill out your copyright notice in the Description page of Project Settings.


#include "TownNpcManager.h"

#include "GuildGame/Characters/CharacterStats.h"
#include "GuildGame/Managers/CharacterManager.h"
#include "GuildGame/Town/TownGameModeBase.h"
#include "TownNavNodeActor.h"
#include "TownNpcCharacter.h"


class ATownNpcAIController;

void UTownNpcManager::BeginDestroy()
{
    Super::BeginDestroy();
}

void UTownNpcManager::ManuelConstructor(UDataTable* Table, ATownGameModeBase* GameMode)
{
    if(Table != nullptr)
    {
        NpcTable = Table;
    }

    if(GameMode != nullptr)
    {
        TownGameMode = GameMode;
    }

}

void UTownNpcManager::StartSpawning(TArray<FCharacterStats*> OwnedCharacters)
{
    for (int i = 0; i < OwnedCharacters.Num(); ++i)
    {
        FVector RandLocation = GetRandomLocationFromNode(100);
        if(TownGameMode && TownGameMode->GetWorld())
        {
           ATownNpcCharacter* SpawnedCharacter =  CharacterManager::SpawnCharacter<ATownNpcCharacter,ATownNpcCharacter>(TownGameMode->NpcCharacterBlueprint,OwnedCharacters[i]->ClassType,
                RandLocation, FRotator{},TownGameMode->GetWorld());
            if(SpawnedCharacter)
            {
                MoveNpcToDestination(SpawnedCharacter);
            }
        }
        //SpawnCharacter(OwnedCharacters[i]->ClassType, OwnedCharacters[i]->TownNpcBehaviourState);
    }
}

void UTownNpcManager::SpawnOnClick()
{
    const int RandIndex = FMath::RandRange(0, NavigationNodes.Num() - 1);
    FVector RandLocation = GetRandomLocationFromNode(100);

    if(TownGameMode && TownGameMode->GetWorld())
    {
        ATownNpcCharacter* SpawnedCharacter = CharacterManager::SpawnCharacter<ATownNpcCharacter,ATownNpcCharacter>(TownGameMode->NpcCharacterBlueprint,ECharacterClassType::Knight,
            RandLocation, FRotator{},TownGameMode->GetWorld());
        if(SpawnedCharacter)
        {
            MoveNpcToDestination(SpawnedCharacter);
        }
    }
}

void UTownNpcManager::SetNpcBehaviourState(int UniqueID, ENpcBehaviourStates State, ECharacterClassType CharacterType)
{
    if(SpawnedNpCs.Contains(UniqueID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Spawn CONTAINS "));
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
        FVector RandLocation = GetRandomLocationFromNode(100);
        
        if(TownGameMode && TownGameMode->GetWorld())
        {
            ATownNpcCharacter* SpawnedCharacter = CharacterManager::SpawnCharacter<ATownNpcCharacter,ATownNpcCharacter>(TownGameMode->NpcCharacterBlueprint,CharacterType,
                RandLocation, FRotator{},TownGameMode->GetWorld());
            if(SpawnedCharacter)
            {
                SpawnedNpCs.Add(UniqueID, SpawnedCharacter);
                MoveNpcToDestination(SpawnedCharacter);
            }
        }
      
    }
}

void UTownNpcManager::MoveNpcToDestination(ATownNpcCharacter* SpawnedActor)
{
    ATownNavNodeActor* RandomNavNode =  GetRandomNavigationNode();
    SpawnedActor->DestinationNode = RandomNavNode;
    SpawnedActor->MoveToRandomLocation();
}

FString UTownNpcManager::GetNpcTableRowName(ECharacterClassType CharacterType)
{
    if(CharacterType == ECharacterClassType::Knight)
    {
        return  FString(TEXT("Knight"));
    }
    else if(CharacterType == ECharacterClassType::Mage)
    {
        return  FString(TEXT("Mage"));
    }
   
    return FString(TEXT("EMPTY"));
}

ATownNavNodeActor* UTownNpcManager::GetRandomNavigationNode()
{
    if(NavigationNodes.Num() <= 0)
    {
        return nullptr;
    }
    
    const int RandIndex = FMath::RandRange(0, NavigationNodes.Num() - 1);
    FVector Location;
    return NavigationNodes[RandIndex];
}

FVector UTownNpcManager::GetRandomLocationFromNode(float Zoffset)
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
