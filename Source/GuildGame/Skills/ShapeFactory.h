// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SkillShape.h"

/**
 * 
 */
class GUILDGAME_API ShapeFactory
{
public:
	static SkillShape* CreateShape(ESkillShapeType Type, const TArray<float>& Params);
};
