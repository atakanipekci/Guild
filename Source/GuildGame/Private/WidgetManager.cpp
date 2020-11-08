#include "WidgetManager.h"

UDataTable* WidgetManager::WidgetsDataTable = nullptr;

void WidgetManager::SetWidgetTable(UDataTable* Table)
{
    UE_LOG(LogTemp, Warning, TEXT("Table Works"));
    if(Table != nullptr)
    {

        WidgetsDataTable = Table;
        
    }
}

TSubclassOf<UUserWidget> WidgetManager::GetWidget(const EWidgetKeys Key)
{
     if(WidgetsDataTable)
    {
        const FString Context(TEXT("CONTEXT Widget Datatable TEXT"));
        const FName Row = *(GetRowName(Key));

        FWidgetsDataTable* WidgetData = WidgetsDataTable->FindRow<FWidgetsDataTable>(Row, Context, true);

        if(WidgetData)
        {
            return  WidgetData->Widget;
        }
    }

    return nullptr;
}

FString WidgetManager::GetRowName(const EWidgetKeys Key)
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

    return FString(TEXT("EMPTY"));
}
