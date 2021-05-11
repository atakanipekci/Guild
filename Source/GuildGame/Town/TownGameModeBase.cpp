// Fill out your copyright notice in the Description page of Project Settings.


#include "TownGameModeBase.h"

#include "GuildGameInstance.h"
#include "GuildGame/Managers/DayTaskManager.h"
#include "GuildGame/Widgets/TownHudWidget.h"
#include "GuildGame/Town/Navigation/TownNpcManager.h"
#include "TownPlayerController.h"
#include "GuildGame/Managers/WidgetManager.h"
#include "GuildGame/Widgets/OwnedCharactersDroppableWidget.h"
#include "Kismet/GameplayStatics.h"

ATownGameModeBase::ATownGameModeBase()
{
    PlayerControllerClass = ATownPlayerController::StaticClass();
}



void ATownGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);
    NpcManager = NewObject<UTownNpcManager>();

}

void ATownGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("GameMode BEGINPLAY "));
    
    DayTaskManager::UpdateTasks(this);

    if(NpcManager)
	{
		NpcManager->ManuelConstructor(NpcTable, this);
        //NpcManager->StartSpawning(OwnedCharacters);
	}

    OnLevelLoaded();    
}

void ATownGameModeBase::OnLevelLoaded()
{
    UGuildGameInstance* GameInstance = Cast<UGuildGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if(GameInstance)
    {
        if(GameInstance->SquadCharacters.Num() > 0)
        {
            for (int i = 0; i < GameInstance->SquadCharacters.Num(); ++i)
            {
                OwnedCharacters.Add(GameInstance->SquadCharacters[i]);
            }
           GameInstance->SquadCharacters.Empty();
        }
    }

    if(OwnedCharactersWidget)
    {
        OwnedCharactersWidget->Refresh();
    }
    
}



void ATownGameModeBase::BeginDestroy()
{
    Super::BeginDestroy();

    WidgetManager::ResetWidgetInstances();

    if(NpcManager)
	{
		NpcManager->ConditionalBeginDestroy();
	}
    
}



void ATownGameModeBase::SpawnOnClick()
{
    if(NpcManager)
    {
       NpcManager->SpawnOnClick();
    }
}

void ATownGameModeBase::SetNpcBehaviourState(int UniqueID, ENpcBehaviourStates State, ECharacterClassType CharacterType)
{
    UE_LOG(LogTemp, Warning, TEXT("SetNpcBehaviourState 1 "));
    if(NpcManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("SetNpcBehaviourState 2 "));
        NpcManager->SetNpcBehaviourState( UniqueID, State, CharacterType);
    }
}

void ATownGameModeBase::UpdateUI(UObject* Caller)
{
    ATownGameModeBase* Mode = Cast<ATownGameModeBase>(UGameplayStatics::GetGameMode(Caller));

    if(Mode)
    {
        if(Mode->MenuWidget)
        {
            Mode->MenuWidget->UpdateUI();
        }
    }
}


