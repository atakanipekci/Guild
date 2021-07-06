// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "GridEffect.h"
#include "GuildGame/Characters/GGCharacter.h"
#include "GuildGame/VFX/GridStatusEffectVfxActor.h"

Grid::Grid(int ArrayIndex, EGridState NewState)
{
    Index = ArrayIndex;
    GridState = NewState;
}

Grid::~Grid()
{
    for (auto It = GridStatusEffects.CreateIterator(); It; ++It)
    {
        delete It.Value();
    }
}

GridEffect* Grid::GetEffect(EEffectType EffectType)
{
    if(GridStatusEffects.Contains(EffectType))
    {
        GridEffect** Effect = GridStatusEffects.Find(EffectType);
        if(Effect)
        {
            return *Effect;
        }
    }
    return nullptr;
}

void Grid::ApplyStatusEffectsToCharacter(AGGCharacter* Char, int GridIndex)
{
    if(GridStatusEffects.Num() <= 0) return;
    
    if(GridIndex == Index)
    {
        for (auto It = GridStatusEffects.CreateIterator(); It; ++It)
        {
            if(It.Value())
            {
                It.Value()->ApplyEffectToCharacter(Char);
            }
        }
    }
}
