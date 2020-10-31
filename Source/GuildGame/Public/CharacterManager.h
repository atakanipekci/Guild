// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GGCharacter.h"
#include "GridManager.h"

/**
 * 
 */
class GUILDGAME_API CharacterManager
{
public:
    static void SetMovableGrids(ACharacter* Character);
    static void SetDamageableGrids(ACharacter* Character);
    static GridManager* CharGridManager;
};
