// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


/**
 * 
 */

enum class EEffectType : uint8;

enum class EGridState
{
	Empty = 0,
	Obstacle = 1,
	// StatusEffect = 2
};

class GUILDGAME_API Grid
{
public:
	Grid(int ArrayIndex, EGridState NewState);
	~Grid();
	int Index;
	EGridState GridState = EGridState::Empty;
	
	TMap<EEffectType, class GridEffect*> GridStatusEffects;
	
	class AGridStatusEffectVfxActor* StatusVisual = nullptr;
	
	GridEffect* GetEffect(EEffectType EffectType);
	void ApplyStatusEffectsToCharacter(class AGGCharacter* Char, int GridIndex);
};
