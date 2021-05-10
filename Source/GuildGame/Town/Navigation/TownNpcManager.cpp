// Fill out your copyright notice in the Description page of Project Settings.


#include "TownNpcManager.h"

#include "CharacterStats.h"
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
        SpawnCharacter(OwnedCharacters[i]->ClassType, OwnedCharacters[i]->TownNpcBehaviourState);
    }
}

void UTownNpcManager::SpawnOnClick()
{
    SpawnCharacter(ECharacterClassType::Knight, ENpcBehaviourStates::WalkingAround);
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
        UE_LOG(LogTemp, Warning, TEXT("Spawn Not CONTAINS "));
        ATownNpcCharacter* SpawnedCharacter = SpawnCharacter(CharacterType,  State);
        if(SpawnedCharacter)
        {
            SpawnedNpCs.Add(UniqueID, SpawnedCharacter);
        }
    }
}

ATownNpcCharacter* UTownNpcManager::SpawnCharacter(ECharacterClassType CharacterType, ENpcBehaviourStates State)
{
    if(NpcTable && TownGameMode)
    {
        UE_LOG(LogTemp, Warning, TEXT("INSIDE SpawnCharacter() "));

         FVector Location(2970.0f, -770.0f, -1210.0f);
        
         FRotator Rotation(0.0f, 0.0f, 0.0f);

        const int RandIndex = FMath::RandRange(0, NavigationNodes.Num() - 1);
        if(NavigationNodes.Num() > 0)
		{
			if(NavigationNodes[RandIndex])
			{
				Location = NavigationNodes[RandIndex]->Destination;
			    Location.Z += 100;
			}
		}
         //FActorSpawnParameters SpawnInfo;
        if(TownGameMode->NpcCharacterBlueprint)
        {
            //ATownNpcPawn* SpawnedActor = TownGameMode->GetWorld()->SpawnActor<ATownNpcPawn>(TownGameMode->NpcPawnBlueprint, Location, Rotation);

            ATownNpcCharacter* SpawnedActor = TownGameMode->GetWorld()->SpawnActor<ATownNpcCharacter>(TownGameMode->NpcCharacterBlueprint, Location, Rotation);
            if(SpawnedActor)
            {
                UE_LOG(LogTemp, Warning, TEXT("BEFORE NAV NODES"));
                if(NavigationNodes.Num() > 0)
		        {
                    UE_LOG(LogTemp, Warning, TEXT("NAVNODES NOT 0"));
                    SpawnedActor->DestinationNode = NavigationNodes[RandIndex];
                    SpawnedActor->MoveToRandomLocation();
		        }
                
                USkeletalMeshComponent* SkeletalMesh = Cast<USkeletalMeshComponent>(SpawnedActor->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
            
                if(SkeletalMesh)
                {
                    const FString Context(TEXT("Widget Row Missing"));
                    const FName Row = *GetNpcTableRowName(CharacterType);
            
                    FNpcDataTable* NpcDataTable = NpcTable->FindRow<FNpcDataTable>(Row, Context, true);
                    
                    if(NpcDataTable)
                    {
                        if(NpcDataTable->SkeletalMesh)
                        {
                            SkeletalMesh->SetSkeletalMesh(NpcDataTable->SkeletalMesh);
                
                            if(NpcDataTable->AnimationBP)
                            {
                                SkeletalMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
                                SkeletalMesh->SetAnimInstanceClass(NpcDataTable->AnimationBP);
                            }
                        }
                    }
                }
            }

            return SpawnedActor;
        }
    }

    return  nullptr;
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
