// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterManager.h"

#include "GuildGame/Characters/GGCharacter.h"
#include "GGLogHelper.h"
#include "GridManager.h"
#include "GuildGame/Characters/CharacterStats.h"
#include "GuildGame/Skills/CharacterSkill.h"
#include "GuildGame/Skills/SkillShape.h"

GridManager* CharacterManager::CharGridManager = nullptr;

void CharacterManager::SetMovableGrids(AGGCharacter* Character)
{
    if(Character == nullptr || CharGridManager == nullptr)
    {
        return;
    }

    int Index = CharGridManager->WorldToGrid(Character->GetActorLocation());
    Character->MovableGrids.Empty();
    CharGridManager->GetGridsInRange(Index,Character->GetDefaultMovementRange(), &(Character->MovableGrids));
}

void CharacterManager::SetTargetableGrids(AGGCharacter* Character, const FSkillData* SkillData)
{
    if(Character == nullptr || CharGridManager == nullptr || SkillData == nullptr)
    {
        return;
    }

    int Index = Character->GetCurrentIndex();
    Character->TargetableGrids.Empty();
    float GridRange = SkillData->Range;
    float DistRange = CharGridManager->GetGridSize()*SkillData->Range;
    TArray<Grid*> UnavailableGrids;
    if(SkillData->DiagonalRange)
    {
        CharGridManager->GetGridsFromCenter(Index, GridRange*2+1, GridRange*2+1, &(Character->TargetableGrids), Character->GetSize() == ECharacterSize::Large);
        UnavailableGrids.Empty();
        GridRange = SkillData->MinRange;
        CharGridManager->GetGridsFromCenter(Index, GridRange*2+1, GridRange*2+1, &UnavailableGrids, Character->GetSize() == ECharacterSize::Large);
    }
    else
    {
        CharGridManager->GetGridsInRange(Index, DistRange, &(Character->TargetableGrids), false);
        UnavailableGrids.Empty();
        DistRange = CharGridManager->GetGridSize()*SkillData->MinRange;
        CharGridManager->GetGridsInRange(Index, DistRange, &UnavailableGrids, false);
        
    }

    for (int i = Character->TargetableGrids.Num()-1; i >= 0; i--)
    {
        Grid* Element = Character->TargetableGrids[i];
        
        if(Element == nullptr)
        {
            Character->TargetableGrids.Remove(Element);
            continue;
        }

        for (auto Element2 : UnavailableGrids)
        {
            if(Element2 == nullptr)
            {
                continue;
            }

            if(Element->Index == Element2->Index)
            {
                Character->TargetableGrids.RemoveAt(i);
            }
        }
    }
    
    /*int Index = CharGridManager->WorldToGrid(Character->GetActorLocation());
    Character->TargetableGrids.Empty();
    CharGridManager->GetGridsInRange(Index,Character->GetDefaultDamageRange(), &(Character->TargetableGrids), false);*/
}

void CharacterManager::SetDamageableGrids(AGGCharacter* Character, const CharacterSkill* Skill, int Index)
{
    if(Character == nullptr || CharGridManager == nullptr || Skill == nullptr)
    {
        return;
    }

    SkillShape* Shape = Skill->GetShape();

    if(Shape == nullptr)
    {
        return;
    }

    Shape->GetGridsInShape(CharGridManager, Index, Character->GetDamageableGrids());
}

bool CharacterManager::CanAttackTo(const AGGCharacter* Dealer, const AGGCharacter* Target)
{
    if(Dealer == nullptr || Target == nullptr)
    {
        return false;
    }

    const FVector2D StartPos (CharGridManager->GetGridCenter(CharGridManager->WorldToGrid(Dealer->GetActorLocation())));
    const FVector2D EndPos (CharGridManager->GetGridCenter(CharGridManager->WorldToGrid(Target->GetActorLocation()))); 

    if(FVector2D::Distance(StartPos,EndPos) > Dealer->GetDefaultDamageRange())
    {
        return false;
    }

    return true;
}

void CharacterManager::SetCharacterGrids(const AGGCharacter* Char, EGridState State)
{
    if(CharGridManager == nullptr || Char == nullptr)
    {
        return;
    }

    int Index = Char->GetCurrentIndex();
    CharGridManager->SetGridState(Index, State);

    if(Char->GetSize() == ECharacterSize::Large)
    {
        FIntPoint Point = CharGridManager->IndexToPoint(Index);
        if((Point - FIntPoint(1,0)).X >= 0)
        {
            int LeftIndex = CharGridManager->PointToIndex(Point - FIntPoint(1,0));
            CharGridManager->SetGridState(LeftIndex, State);
        }

        if((Point - FIntPoint(1,1)).X >= 0 && (Point - FIntPoint(1,1)).Y >= 0)
        {
            int BotLeftIndex = CharGridManager->PointToIndex(Point - FIntPoint(1,1));
            CharGridManager->SetGridState(BotLeftIndex, State);
        }

        if((Point - FIntPoint(0,1)).Y >= 0)
        {
            int BotIndex = CharGridManager->PointToIndex(Point - FIntPoint(0,1));
            CharGridManager->SetGridState(BotIndex, State);
        }
    }
}

FString CharacterManager::GetCharFileRowName(ECharacterClassType CharacterType)
{
    if(CharacterType == ECharacterClassType::Knight)
    {
        return  FString(TEXT("Knight"));
    }
    else if(CharacterType == ECharacterClassType::Mage)
    {
        return  FString(TEXT("Mage"));
    }
    else if(CharacterType == ECharacterClassType::Archer)
    {
        return  FString(TEXT("Archer"));
    }
   
    return FString(TEXT("EMPTY"));
}
