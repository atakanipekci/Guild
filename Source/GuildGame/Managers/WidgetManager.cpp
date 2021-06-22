#include "WidgetManager.h"

#include "CharacterManager.h"
#include "Blueprint/UserWidget.h"

UDataTable* WidgetManager::WidgetsDataTable = nullptr;
UDataTable* WidgetManager::SequenceDataTable = nullptr;

int WidgetManager::SpawnedDraggableWidgetCount = 0;

TMap<EWidgetKeys, UUserWidget* > WidgetManager::WidgetInstanceMap;

void WidgetManager::SetWidgetTable(UDataTable* Table)
{
    if(Table != nullptr)
    {
        WidgetsDataTable = Table;
    }

    WidgetInstanceMap.Empty();
}

void WidgetManager::SetSequenceTable(UDataTable* Table)
{
    if(Table != nullptr)
    {
        SequenceDataTable = Table;
    }
}

TSubclassOf<UUserWidget> WidgetManager::GetWidget(const EWidgetKeys Key)
{
     if(WidgetsDataTable)
    {
        const FString Context(TEXT("Widget Row Missing"));
        const FName Row = *(GetWidgetRowName(Key));

        FWidgetsDataTable* WidgetData = WidgetsDataTable->FindRow<FWidgetsDataTable>(Row, Context, true);

        if(WidgetData)
        {
            return  WidgetData->Widget;
        }
    }

    return nullptr;
}

FString WidgetManager::GetWidgetRowName(const EWidgetKeys Key)
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
    
    return FString(TEXT("EMPTY"));
}



void WidgetManager::SetWidgetInstance(const EWidgetKeys Key, UUserWidget* Widget)
{
    /*if(WidgetInstanceMap.Contains(Key))
    {
        WidgetInstanceMap.Remove(Key);
    }*/
    WidgetInstanceMap.Add(Key, Widget);
}

UUserWidget* WidgetManager::GetWidgetInstance(const EWidgetKeys Key)
{
    UUserWidget** Widget = WidgetInstanceMap.Find(Key);
    if(Widget)
    {
        return  *Widget;
    }
    return  nullptr;
}

UUserWidget* WidgetManager::GetWidgetInstanceIfNotCreate(const EWidgetKeys Key, UObject* Owner)
{
     UUserWidget* NewWidget = GetWidgetInstance(Key);
     if(NewWidget == nullptr)
     {
         NewWidget = CreateWidgetInstance(Key, Owner);
         SetWidgetInstance(Key, NewWidget);
     }
    return  NewWidget;
}

UUserWidget* WidgetManager::CreateWidgetInstance(const EWidgetKeys Key, UObject* Owner)
{
     return CreateWidget<UUserWidget>(Owner->GetWorld(), GetWidget(Key));
}

UUserWidget* WidgetManager::GetSkillsWidgetByType(ECharacterClassType ClassType, UObject* Owner)
{
    if(ClassType == ECharacterClassType::Knight)
    {
        return GetWidgetInstanceIfNotCreate(EWidgetKeys::KnightSkills, Owner);
    }
    else if(ClassType == ECharacterClassType::Mage)
    {
        return GetWidgetInstanceIfNotCreate(EWidgetKeys::MageSkills, Owner);
    }
    else if(ClassType == ECharacterClassType::Archer)
    {
        return GetWidgetInstanceIfNotCreate(EWidgetKeys::ArcherSkills, Owner);
    }
    else
    {
        return  nullptr;
    }
}

FString WidgetManager::GetSequenceRowName(const ESequenceKeys Key)
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

ULevelSequence* WidgetManager::GetSequence(const ESequenceKeys Key)
{
     if(SequenceDataTable)
    {
        const FString Context(TEXT("Sequence Row Missing"));
        const FName Row = *(GetSequenceRowName(Key));
        FSequenceDataTable* WidgetData = SequenceDataTable->FindRow<FSequenceDataTable>(Row, Context, true);

        if(WidgetData)
        {
            return  WidgetData->SequenceAsset;
        }
    }

    return nullptr;
}

void WidgetManager::ResetWidgetInstances()
{
    WidgetInstanceMap.Empty();
}

int WidgetManager::IncrementSpawnedDraggableCount()
{
    return SpawnedDraggableWidgetCount++;
}
