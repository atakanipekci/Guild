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
        CharGridManager->GetGridsFromCenter(Index, GridRange*2+1, GridRange*2+1, &(Character->TargetableGrids));
        UnavailableGrids.Empty();
        GridRange = SkillData->MinRange;
        CharGridManager->GetGridsFromCenter(Index, GridRange*2+1, GridRange*2+1, &UnavailableGrids);
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
   
    return FString(TEXT("EMPTY"));
}
