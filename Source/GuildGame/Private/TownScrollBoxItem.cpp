// Fill out your copyright notice in the Description page of Project Settings.


#include "TownScrollBoxItem.h"


#include "TownGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UTownScrollBoxItem::NativeConstruct()
{
    Super::NativeConstruct();

    ATownGameModeBase* GameMode = Cast<ATownGameModeBase>(UGameplayStatics::GetGameMode(this));

    if(GameMode->CharactersTable)
    {
        const FString Context(TEXT("CONTEXT DATATABLE TEXT"));
        const FName Row = *(FString(TEXT("Knight")));
        FCharacterStats* CharacterData = GameMode->CharactersTable->FindRow<FCharacterStats>(Row, Context, true);

        if(CharacterData)
        {
            FCharacterStats* CopyStruct = new FCharacterStats(*CharacterData);
            CopyStruct->Price = 50;
            Stat = CopyStruct;
        }
    }
}

void UTownScrollBoxItem::SetScrollType(EScrollBoxType Type)
{
    ScrollType = Type;
}

void UTownScrollBoxItem::SetPrevParentType(EScrollBoxType Type, UPanelWidget* PrevParent)
{
    PrevParentType = Type;
    PrevParentWidget = PrevParent;
}
