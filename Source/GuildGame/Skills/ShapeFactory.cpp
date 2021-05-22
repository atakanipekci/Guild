// Fill out your copyright notice in the Description page of Project Settings.


#include "ShapeFactory.h"

SkillShape* ShapeFactory::CreateShape(ESkillShapeType Type, const TArray<float>& Params)
{
	switch(Type)
	{
		case ESkillShapeType::Circle:
			if(Params.Num() < 1)
			{
				return nullptr;
			}
			return new ShapeCircle(Params[0]);
			break;

		case ESkillShapeType::Rectangle:
			if(Params.Num() < 2)
			{
				return nullptr;
			}
			return new ShapeRectangle(Params[0], Params[1]);
			break;

		default:
			if(Params.Num() < 1)
			{
				return nullptr;
			}
			return new ShapeCircle(Params[0]);
			break;
	}
}
