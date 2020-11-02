// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterManager.h"

#include "GGCharacter.h"
#include "GGLogHelper.h"
#include "GridManager.h"

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

void CharacterManager::SetDamageableGrids(AGGCharacter* Character)
{
    if(Character == nullptr || CharGridManager == nullptr)
    {
        return;
    }
    

    int Index = CharGridManager->WorldToGrid(Character->GetActorLocation());
    Character->DamageableGrids.Empty();
    CharGridManager->GetGridsInRange(Index,Character->GetDefaultDamageRange(), &(Character->DamageableGrids), false);
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
