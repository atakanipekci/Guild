// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterManager.h"
#include "GGLogHelper.h"

GridManager* CharacterManager::CharGridManager = nullptr;

void CharacterManager::SetMovableGrids(ACharacter* Character)
{
    if(Character == nullptr || CharGridManager == nullptr)
    {
        return;
    }
    AGGCharacter * GGChar = Cast<AGGCharacter>(Character);
    if(GGChar == nullptr)
    {
        return;
    }

    int Index = CharGridManager->WorldToGrid(GGChar->GetActorLocation());
    GGChar->MovableGrids.Empty();
    CharGridManager->GetGridsInRange(Index,GGChar->GetDefaultMovementRange(), &(GGChar->MovableGrids));
}

void CharacterManager::SetDamageableGrids(ACharacter* Character)
{
    if(Character == nullptr || CharGridManager == nullptr)
    {
        return;
    }
    AGGCharacter * GGChar = Cast<AGGCharacter>(Character);
    if(GGChar == nullptr)
    {
        return;
    }

    int Index = CharGridManager->WorldToGrid(GGChar->GetActorLocation());
    GGChar->DamageableGrids.Empty();
    CharGridManager->GetGridsInRange(Index,GGChar->GetDefaultDamageRange(), &(GGChar->DamageableGrids), false);
}
