#include "WidgetManager.h"

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
    else if(Key == EWidgetKeys::TavernMenuWidget)
    {
        return  FString(TEXT("TavernWidget"));
    }
    else if(Key == EWidgetKeys::RecruitMenuWidget)
    {
        return  FString(TEXT("RecruitWidget"));
    }

    return FString(TEXT("EMPTY"));
}



void WidgetManager::SetWidgetInstance(const EWidgetKeys Key, UUserWidget* Widget)
{
    if(WidgetInstanceMap.Contains(Key))
    {
        WidgetInstanceMap.Remove(Key);
    }
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

int WidgetManager::GetAndSetDraggableSpawnCount()
{
    return SpawnedDraggableWidgetCount++;
}
