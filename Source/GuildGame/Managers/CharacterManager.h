// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class GUILDGAME_API CharacterManager
{
public:
    static void SetMovableGrids(class AGGCharacter* Character);
    static void SetDamageableGrids(class AGGCharacter* Character);
    static bool CanAttackTo(const class AGGCharacter*, const class AGGCharacter* );
    static class GridManager* CharGridManager;
};
