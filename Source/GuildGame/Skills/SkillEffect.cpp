// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillEffect.h"

#include "GuildGame/GuildGameGameModeBase.h"
#include "GuildGame/Managers/StatusEffectManager.h"
#include "GuildGame/Managers/TimedEventManager.h"
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
		ATimedEventManager::SpawnPopupText(EPopupTextType::Lucky, 0, 2, Character->GetActorLocation(), Character->GetWorld(), false);
		Result *= 2;
	}
	int Damage = Owner->GetBaseDamage()*Result/100;
	Character->TakeDefaultDamage(Damage, Owner);
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
		ATimedEventManager::SpawnPopupText(EPopupTextType::Lucky, 0, 2, Character->GetActorLocation(), Character->GetWorld(), false);
		Total *= 2;
	}
	int Physical = Total*(100-Data.ExtraValue)/100;
	int True = Total - Physical;

	int PhysicalDamage = Owner->GetBaseDamage()*Physical/100;
	int TrueDamage = Owner->GetBaseDamage()*True/100;

	Character->TakePhysicalDamage(PhysicalDamage, Owner);
	Character->TakeDefaultDamage(TrueDamage, Owner);
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
		ATimedEventManager::SpawnPopupText(EPopupTextType::Lucky, 0, 2, Character->GetActorLocation(), Character->GetWorld(), false);
		Total *= 2;
	}
	int Magical = Total*(100-Data.ExtraValue)/100;
	int True = Total - Magical;

	int MagicalDamage = Owner->GetBaseDamage()*Magical/100;
	int TrueDamage = Owner->GetBaseDamage()*True/100;
	
	
	Character->TakeMagicalDamage(MagicalDamage, Owner);
	Character->TakeDefaultDamage(TrueDamage, Owner);
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
		ATimedEventManager::SpawnPopupText(EPopupTextType::Lucky, 0, 2, Character->GetActorLocation(), Character->GetWorld(), false);
		Result *= 2;
	}

	if(Result > 0)
	{
		ATimedEventManager::SpawnPopupText(EPopupTextType::Heal, Result, 2, Character->GetActorLocation(), Character->GetWorld());
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

EffectGridStatus::EffectGridStatus(const FEffectData& EffectData, AGGCharacter* OwnerChar)
{
	Data = EffectData;
	Owner = OwnerChar;
}

bool EffectGridStatus::ApplyEffectToCharacter(AGGCharacter* Character)
{
	if(Character == nullptr || Owner == nullptr)
	{
		return false;
	}

	return  true;
}
