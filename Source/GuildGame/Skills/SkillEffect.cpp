// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillEffect.h"
#include "Math/UnrealMathUtility.h"


EffectDealDamage::EffectDealDamage(const FEffectData& EffectData)
{
	Data = EffectData;
}

bool EffectDealDamage::ApplyEffectToCharacter(AGGCharacter* Character)
{
	if(Character == nullptr)
	{
		return false;
	}

	int Result = FMath::RandRange(Data.MinValue, Data.MaxValue);
	Character->TakeDefaultDamage(Result, nullptr);
	return true;
}

EffectHeal::EffectHeal(const FEffectData& EffectData)
{
	Data = EffectData;
}

bool EffectHeal::ApplyEffectToCharacter(AGGCharacter* Character)
{
	if(Character == nullptr)
	{
		return false;
	}

	int Result = FMath::RandRange(Data.MinValue, Data.MaxValue);
	Character->Heal(Result,nullptr);
	return true;
	
}
