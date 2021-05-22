// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectFactory.h"

SkillEffect* EffectFactory::CreateEffect(const FEffectData& Data)
{
	switch(Data.Type)
	{
		case EEffectType::DealDamage:
			return new EffectDealDamage(Data);
			break;

		case EEffectType::Heal:
			return new EffectHeal(Data);
			break;

		default:
			return nullptr;
			break;
	}
}
