// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillEffect.h"

#include "GuildGame/GuildGameGameModeBase.h"
#include "GuildGame/Managers/StatusEffectManager.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"


EffectDealDamage::EffectDealDamage(const FEffectData& EffectData, AGGCharacter* OwnerChar)
{
	Data = EffectData;
	Owner = OwnerChar;
}

bool EffectDealDamage::ApplyEffectToCharacter(AGGCharacter* Character)
{
	if(Character == nullptr || Owner == nullptr)
	{
		return false;
	}

	int CritChance = FMath::RandRange(0, 100);

	int Result = FMath::RandRange(Data.MinValue, Data.MaxValue);

	if(CritChance < Data.CritChance)
	{
		Result *= 2;
	}
	else if(CritChance < Data.CritChance + Owner->GetLuck()*2 - Character->GetLuck()*2)
	{
		//Lucky
		Result *= 2;
	}
	Character->TakeDefaultDamage(Owner->GetBaseDamage()*Result/100, Owner);
	return true;
}

EffectDealPhysicalDamage::EffectDealPhysicalDamage(const FEffectData& EffectData, AGGCharacter* OwnerChar)
{
	Data = EffectData;
	Owner = OwnerChar;
}

bool EffectDealPhysicalDamage::ApplyEffectToCharacter(AGGCharacter* Character)
{
	if(Character == nullptr || Owner == nullptr)
	{
		return false;
	}
	int CritChance = FMath::RandRange(0, 100);
	
	int Total = FMath::RandRange(Data.MinValue, Data.MaxValue);
	if(CritChance < Data.CritChance)
	{
		Total *= 2;
	}
	else if(CritChance < Data.CritChance + Owner->GetLuck()*2 - Character->GetLuck()*2)
	{
		//Lucky
		Total *= 2;
	}
	int Physical = Total*(100-Data.ExtraValue)/100;
	int True = Total - Physical;
	Character->TakePhysicalDamage(Owner->GetBaseDamage()*Physical/100, Owner);
	Character->TakeDefaultDamage(Owner->GetBaseDamage()*True/100, Owner);
	return true;
}

EffectDealMagicalDamage::EffectDealMagicalDamage(const FEffectData& EffectData, AGGCharacter* OwnerChar)
{
	Data = EffectData;
	Owner = OwnerChar;
}

bool EffectDealMagicalDamage::ApplyEffectToCharacter(AGGCharacter* Character)
{
	if(Character == nullptr || Owner == nullptr)
	{
		return false;
	}
	int CritChance = FMath::RandRange(0, 100);
	
	int Total = FMath::RandRange(Data.MinValue, Data.MaxValue);
	if(CritChance < Data.CritChance)
	{
		Total *= 2;
	}
	else if(CritChance < Data.CritChance + Owner->GetLuck()*2 - Character->GetLuck()*2)
	{
		//Lucky
		Total *= 2;
	}
	int Magical = Total*(100-Data.ExtraValue)/100;
	int True = Total - Magical;
	Character->TakeMagicalDamage(Owner->GetBaseDamage()*Magical/100, Owner);
	Character->TakeDefaultDamage(Owner->GetBaseDamage()*True/100, Owner);
	return true;
}

EffectHeal::EffectHeal(const FEffectData& EffectData, AGGCharacter* OwnerChar)
{
	Data = EffectData;
	Owner = OwnerChar;
}

bool EffectHeal::ApplyEffectToCharacter(AGGCharacter* Character)
{
	if(Character == nullptr)
	{
		return false;
	}
	int CritChance = FMath::RandRange(0, 100);
	
	int Result = FMath::RandRange(Data.MinValue, Data.MaxValue);
	if(CritChance < Data.CritChance)
	{
		Result *= 2;
	}
	else if(CritChance < Data.CritChance + Owner->GetLuck()*2 - Character->GetLuck()*2)
	{
		//Lucky
		Result *= 2;
	}
	Character->Heal(Result,Owner);
	return true;
	
}

EffectStatus::EffectStatus(const FEffectData& EffectData, AGGCharacter* OwnerChar)
{
	Data = EffectData;
	Owner = OwnerChar;
}

bool EffectStatus::ApplyEffectToCharacter(AGGCharacter* Character)
{
	if(Character == nullptr || Owner == nullptr)
	{
		return false;
	}
	
	StatusEffectManager::AddStatusEffect(Character, Owner, &Data);
	AGuildGameGameModeBase* BattleGameMode = Cast<AGuildGameGameModeBase>(UGameplayStatics::GetGameMode(Owner));
	if(BattleGameMode)
	{
		BattleGameMode->BattleTurnManager.UpdateWidgetOrder(true);
	}
	
	Character->UpdateHealthBarStatusEffects();
	
	return true;
	
}
