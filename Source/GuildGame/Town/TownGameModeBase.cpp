// Fill out your copyright notice in the Description page of Project Settings.


#include "TownGameModeBase.h"

#include "BuildingStats.h"
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
		NpcManager->ManuelConstructor(/*NpcTable, */this);
        //NpcManager->StartSpawning(OwnedCharacters);
	}

    HudWidgetInstance = Cast<UTownHudWidget>(WidgetManager::GetOrCreateWidgetInstance(EWidgetKeys::TownHud, this));
    if(HudWidgetInstance)
    {
        HudWidgetInstance->AddToViewport();
        HudBuildingWidgetsContainerOverlay = HudWidgetInstance->BuildingWidgetsContainerOverlay;
    }

    
    FRecruitBuildingStats* NewRecruitBuildingStats = static_cast<FRecruitBuildingStats*>(CreateBuildingStats(EBuildingTypes::Recruit));
    if(NewRecruitBuildingStats)
    {
        NewRecruitBuildingStats->BuildingType = EBuildingTypes::Recruit;
        BuildingStatsMap.Add(EBuildingTypes::Recruit, NewRecruitBuildingStats);
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

    if(OwnedCharsDroppableWidgetInstance)
    {
        OwnedCharsDroppableWidgetInstance->Refresh();
    }

    UpdateTownHud(this);
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

FBuildingStatsBase* ATownGameModeBase::CreateBuildingStats(EBuildingTypes BuildingType)
{
    if(BuildingType == EBuildingTypes::Recruit)
    {
        const TArray<FName> RowNames = RecruitBuildingUpgradeTable->GetRowNames();
        const int RowCount = RowNames.Num();
        
        FRecruitBuildingStats* BuildingStats = new FRecruitBuildingStats();
        
        const int TargetNameIndex = FMath::Max(BuildingStats->UpgradeLevel - 1, 0);
        
        if(RowCount > 0 && TargetNameIndex < RowCount)
        {
            //TODO Load Recuit Data here
            const FString Context(TEXT("Buildin Upgrade Stat Row Missing"));
            const FName Row = RowNames[TargetNameIndex];

            FRecruitBuildingUpgradeStats* UpgradeData = RecruitBuildingUpgradeTable->FindRow<FRecruitBuildingUpgradeStats>(Row, Context, true);
            if(UpgradeData)
            {
                FRecruitBuildingUpgradeStats* CopyStruct = new FRecruitBuildingUpgradeStats(*UpgradeData);
                
                BuildingStats->UpgradeStats = CopyStruct;
                return  BuildingStats;
            }
        }
    }

    return  nullptr;
}

void ATownGameModeBase::UpgradeBuilding(EBuildingTypes BuildingType, bool& OutIsNextUpdateAvailable)
{
    UE_LOG(LogTemp, Warning, TEXT("UPDATED 0.1"));
    if(BuildingType == EBuildingTypes::Recruit)
    {
        const TArray<FName> RowNames = RecruitBuildingUpgradeTable->GetRowNames();
        const int RowCount = RowNames.Num();
        
        FRecruitBuildingStats* BuildingStats = static_cast<FRecruitBuildingStats*>(BuildingStatsMap[EBuildingTypes::Recruit]);

        UE_LOG(LogTemp, Warning, TEXT("UPDATED 0.2"));
        if(BuildingStats)
        {
             UE_LOG(LogTemp, Warning, TEXT("UPDATED 0.3 %d"), RowCount);
            if(RowCount > 0 &&  BuildingStats->UpgradeLevel  < RowCount)
            {
                 const int TargetNameIndex = BuildingStats->UpgradeLevel;
                 BuildingStats->UpgradeLevel++;
                 UE_LOG(LogTemp, Warning, TEXT("UPDATED1"));

                 const FString Context(TEXT("Buildin Upgrade Stat Row Missing"));
                 const FName Row = RowNames[TargetNameIndex];

                 FRecruitBuildingUpgradeStats* UpgradeData = RecruitBuildingUpgradeTable->FindRow<FRecruitBuildingUpgradeStats>(Row, Context, true);
                 if(UpgradeData)
                 {
                     UE_LOG(LogTemp, Warning, TEXT("UPDATED3"));
                     FRecruitBuildingUpgradeStats* CopyStruct = new FRecruitBuildingUpgradeStats(*UpgradeData);

                     if(BuildingStats->UpgradeStats)
                         delete BuildingStats->UpgradeStats;

                     BuildingStats->UpgradeStats = CopyStruct;

                     if(BuildingStats->UpgradeLevel < RowCount)
                     {
                         OutIsNextUpdateAvailable = true;
                     }
                 }
            }
        }
    }
}

void ATownGameModeBase::UpdateTownHud(UObject* Caller)
{
    ATownGameModeBase* Mode = Cast<ATownGameModeBase>(UGameplayStatics::GetGameMode(Caller));

    if(Mode)
    {
        if(Mode->HudWidgetInstance)
        {
            Mode->HudWidgetInstance->UpdateUI();
        }
    }
}


