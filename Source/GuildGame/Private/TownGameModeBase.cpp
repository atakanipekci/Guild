// Fill out your copyright notice in the Description page of Project Settings.


#include "TownGameModeBase.h"

#include "DayTaskManager.h"
#include "TownMenuWidget.h"
#include "TownNpcManager.h"
#include "TownPlayerController.h"
#include "WidgetManager.h"
#include "Kismet/GameplayStatics.h"

ATownGameModeBase::ATownGameModeBase()
{
    PlayerControllerClass = ATownPlayerController::StaticClass();
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
    if(NpcManager)
    {
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


