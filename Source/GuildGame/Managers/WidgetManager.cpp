#include "WidgetManager.h"

#include "CharacterManager.h"
#include "Blueprint/UserWidget.h"

int AWidgetManager::SpawnedDraggableWidgetCount = 0;

AWidgetManager* AWidgetManager::Instance = nullptr;

void AWidgetManager::BeginDestroy()
{
    Super::BeginDestroy();
    Instance = nullptr;
}

void AWidgetManager::CreateManagerInstance(UWorld* World)
{
    if(World == nullptr) return;
    
	if(Instance == nullptr)
	{
		Instance = World->SpawnActor<AWidgetManager>();

	    UGuildGameInstance* GameInstance = Cast<UGuildGameInstance>(UGameplayStatics::GetGameInstance(World));
	    if(GameInstance && Instance)
	    {
	        Instance->WidgetsDataTable = GameInstance->WidgetsTable;
	        Instance->SequenceDataTable = GameInstance->SequenceTable;
	    }
	    // for (auto It = Instance->WidgetInstanceMap.CreateIterator(); It; ++It)
     //    {
     //        It->Value.SetWorld(World);
     //    }
	}

}

TSubclassOf<UUserWidget> AWidgetManager::GetWidget(const EWidgetKeys Key, UWorld* World)
{
    if(World == nullptr) return nullptr;
    
    if(Instance == nullptr)
    {
        CreateManagerInstance(World);
    }
     if(Instance  &&  Instance->WidgetsDataTable)
    {
        const FString Context(TEXT("Widget Row Missing"));
        const FName Row = *(GetWidgetRowName(Key));

        FWidgetsDataTable* WidgetData = Instance->WidgetsDataTable->FindRow<FWidgetsDataTable>(Row, Context, true);

        if(WidgetData)
        {
            return  WidgetData->Widget;
        }
    }

    return nullptr;
}

FString AWidgetManager::GetWidgetRowName(const EWidgetKeys Key)
{
    if(Key == EWidgetKeys::YesOrNo)
    {
        return  FString(TEXT("YesNoWidget"));
    }
    else if(Key == EWidgetKeys::DraggedOwnedWidget)
    {
        return  FString(TEXT("DraggedOwnedWidget"));
    }
    else if(Key == EWidgetKeys::DraggedSquadWidget)
    {
        return  FString(TEXT("DraggedSquadWidget"));
    }
     else if(Key == EWidgetKeys::DraggedRecruitWidget)
    {
        return  FString(TEXT("DraggedRecruitWidget"));
    }
    else if(Key == EWidgetKeys::TavernMenuWidget)
    {
        return  FString(TEXT("TavernWidget"));
    }
    else if(Key == EWidgetKeys::RecruitMenuWidget)
    {
        return  FString(TEXT("RecruitWidget"));
    }
    else if(Key == EWidgetKeys::RecruitUpgradeWidget)
    {
        return  FString(TEXT("RecruitUpgradeWidget"));
    }
    else if(Key == EWidgetKeys::CharacterDetail)
    {
        return  FString(TEXT("CharacterDetail"));
    }
    else if(Key == EWidgetKeys::GuildDetail)
    {
        return  FString(TEXT("GuildDetail"));
    }
    else if(Key == EWidgetKeys::TownHud)
    {
        return FString(TEXT("TownHud"));
    }
     else if(Key == EWidgetKeys::BattleHud)
    {
        return FString(TEXT("BattleHud"));
    }
    else if(Key == EWidgetKeys::RecruitBuildingUpgTooltip)
    {
        return FString(TEXT("RecruitBuildingUpgTooltip"));
    }
    else if(Key == EWidgetKeys::KnightSkills)
    {
        return FString(TEXT("KnightSkills"));
    }
    else if(Key == EWidgetKeys::MageSkills)
    {
        return FString(TEXT("KnightSkills"));
    }
    else if(Key == EWidgetKeys::ArcherSkills)
    {
        return FString(TEXT("KnightSkills"));
    }
    else if(Key == EWidgetKeys::CharacterSkillTooltip)
    {
        return FString(TEXT("CharacterSkillTooltip"));
    }
    else if(Key == EWidgetKeys::StatusEffectNodeWidget)
    {
        return FString(TEXT("StatusEffectNodeWidget"));
    }
    else if(Key == EWidgetKeys::StatusEffectTooltip)
    {
        return FString(TEXT("StatusEffectTooltip"));
    }
    else if(Key == EWidgetKeys::StatusEffectStackableTooltip)
    {
        return FString(TEXT("StatusEffectStackableTooltip"));
    }
     else if(Key == EWidgetKeys::TurnInfoNode)
    {
        return FString(TEXT("TurnInfoNode"));
    }
    
    return FString(TEXT("EMPTY"));
}



void AWidgetManager::SetWidgetInstance(const EWidgetKeys Key, UUserWidget* Widget, UWorld* World)
{
    /*if(WidgetInstanceMap.Contains(Key))
    {
        WidgetInstanceMap.Remove(Key);
    }*/

    if(World == nullptr) return;
    
    if(Instance == nullptr)
    {
        CreateManagerInstance(World);
    }

    if(Instance)
    {
        Instance->WidgetInstanceMap.Add(Key, Widget);
    }
}

UUserWidget* AWidgetManager::GetWidgetInstance(const EWidgetKeys Key, UWorld* World)
{
    if(World == nullptr) return nullptr;
    
    if(Instance == nullptr)
    {
        CreateManagerInstance(World);
    }
    if (Instance)
    {
        UUserWidget** Widget = Instance->WidgetInstanceMap.Find(Key);
        if(Widget)
        {
            return  *Widget;
        }
    }
    return  nullptr;
}

UUserWidget* AWidgetManager::GetOrCreateWidgetInstance(const EWidgetKeys Key, UWorld* World)
{
    if(World == nullptr) return nullptr;
    
    if(Instance == nullptr)
    {
        CreateManagerInstance(World);
    }
    
    UUserWidget* NewWidget = GetWidgetInstance(Key, World);
    if(NewWidget == nullptr)
    {
        NewWidget = CreateWidgetInstance(Key, World);
        SetWidgetInstance(Key, NewWidget, World);
    }
    return  NewWidget;
}

UUserWidget* AWidgetManager::CreateWidgetInstance(const EWidgetKeys Key, UWorld* World)
{
    if(World == nullptr) return nullptr;
    
     return CreateWidget<UUserWidget>(World, GetWidget(Key, World));
}

UUserWidget* AWidgetManager::GetSkillsWidgetByType(ECharacterClassType ClassType, UWorld* World)
{
    if(World == nullptr) return nullptr;
    
    if(ClassType == ECharacterClassType::Knight)
    {
        return GetOrCreateWidgetInstance(EWidgetKeys::KnightSkills, World);
    }
    if(ClassType == ECharacterClassType::Mage)
    {
        return GetOrCreateWidgetInstance(EWidgetKeys::MageSkills, World);
    }
    if(ClassType == ECharacterClassType::Archer)
    {
        return GetOrCreateWidgetInstance(EWidgetKeys::ArcherSkills, World);
    }
    
    return  nullptr;
}

FString AWidgetManager::GetSequenceRowName(const ESequenceKeys Key)
{
    if(Key == ESequenceKeys::TavernMenu)
    {
        return  FString(TEXT("TavernMenu"));
    }
    else if(Key == ESequenceKeys::RecruitMenu)
    {
        return  FString(TEXT("RecruitMenu"));
    }

    return FString(TEXT("EMPTY"));
}

ULevelSequence* AWidgetManager::GetSequence(const ESequenceKeys Key, UWorld* World)
{
    if(World == nullptr) return nullptr;

     if(Instance == nullptr)
     {
         CreateManagerInstance(World);
     }
    
     if(Instance && Instance->SequenceDataTable)
    {
        const FString Context(TEXT("Sequence Row Missing"));
        const FName Row = *(GetSequenceRowName(Key));
        FSequenceDataTable* WidgetData = Instance->SequenceDataTable->FindRow<FSequenceDataTable>(Row, Context, true);

        if(WidgetData)
        {
            return  WidgetData->SequenceAsset;
        }
    }

    return nullptr;
}

int AWidgetManager::IncrementSpawnedDraggableCount()
{
    return SpawnedDraggableWidgetCount++;
}
