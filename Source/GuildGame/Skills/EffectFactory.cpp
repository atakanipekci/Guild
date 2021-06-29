// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectFactory.h"
#include "GuildGame/Characters/GGCharacter.h"


SkillEffect* EffectFactory::CreateEffect(const FEffectData& Data, AGGCharacter* Owner)
{
	switch(Data.Type)
	{
		case EEffectType::DealDamage:
			return new EffectDealDamage(Data, Owner);
			break;

		case EEffectType::DealPhysicalDamage:
			return new EffectDealPhysicalDamage(Data, Owner);
			break;

		case EEffectType::DealMagicalDamage:
			return new EffectDealMagicalDamage(Data, Owner);
			break;

		case EEffectType::Heal:
			return new EffectHeal(Data, Owner);
			break;

		default:
			return nullptr;
			break;
	}
}
