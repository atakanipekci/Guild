// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectFactory.h"
#include "GuildGame/Characters/GGCharacter.h"


SkillEffect* EffectFactory::CreateEffect(const FEffectData& Data, AGGCharacter* Owner)
{
	if(Data.Type == EEffectType::DealDamage)
	{
		return new EffectDealDamage(Data, Owner);
	}
	else if(Data.Type == EEffectType::DealPhysicalDamage)
	{
		return new EffectDealPhysicalDamage(Data, Owner);
	}
	else if(Data.Type == EEffectType::DealMagicalDamage)
	{
		return new EffectDealMagicalDamage(Data, Owner);
	}
	else if(Data.Type == EEffectType::Heal)
	{
		return new EffectHeal(Data, Owner);
	}
	else if(Data.Type == EEffectType::StatusStun || Data.Type == EEffectType::StatusBleed || Data.Type == EEffectType::StatusPoison || Data.Type == EEffectType::StatusBurn
		|| Data.Type == EEffectType::StatusBuff || Data.Type == EEffectType::StatusDeBuff || Data.Type == EEffectType::StatusHeal)
	{
		return new EffectStatus(Data, Owner);
	}
	else if(Data.Type == EEffectType::GridStatusBurn)
	{
		return new EffectGridStatus(Data, Owner);
	}

	return nullptr;

}
