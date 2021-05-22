// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SkillEffect.h"

/**
 * 
 */
class GUILDGAME_API EffectFactory
{
public:
	static SkillEffect* CreateEffect(const FEffectData&);
};
