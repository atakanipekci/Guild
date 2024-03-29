// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSkill.h"

#include "ShapeFactory.h"
#include "EffectFactory.h"
#include "GGLogHelper.h"
#include "GuildGameInstance.h"
#include "GuildGame/Managers/StatusEffectManager.h"
#include "GuildGame/Managers/TimedEventManager.h"
#include "Kismet/GameplayStatics.h"

CharacterSkill::CharacterSkill(const FSkillData& Data, const FCharSkillFileDataTable& File, AGGCharacter* CharOwner)
{
	SkillData = Data;
	SkillFile = File;
	Owner = CharOwner;

	SkillID = SkillData.SkillID;
	
	Shape = ShapeFactory::CreateShape(Data.ShapeType, Data.ShapeParameters);
	for (auto& Element : Data.EffectData)
	{
		SkillEffects.Add(EffectFactory::CreateEffect(Element, Owner));		
	}
}

CharacterSkill::~CharacterSkill()
{
	delete Shape;
	for (auto Element : SkillEffects)
	{
		delete Element;
	}
}

void CharacterSkill::ApplyEffects(AGGCharacter* Caster, TArray<AGGCharacter*>& TargetCharacters)
{
	for (auto TargetCharacter : TargetCharacters)
	{
		int HitChance = FMath::RandRange(0, 100);
		if(HitChance > Caster->GetAccuracy() - TargetCharacter->GetDodge())
		{
			//dodged
			ATimedEventManager::SpawnPopupText(EPopupTextType::Dodged, 0, 2, TargetCharacter->GetActorLocation(), TargetCharacter->GetWorld(), false);
			continue;
		}

		for (auto Effect : SkillEffects)
		{
			int EffectChance = FMath::RandRange(0,100);
			if(EffectChance > Effect->GetEffectData().Chance)
			{
				//missed effect
				ATimedEventManager::SpawnPopupText(EPopupTextType::MissedEffect, 0, 2, TargetCharacter->GetActorLocation(), TargetCharacter->GetWorld(), false);
				continue;
			}

			if(StatusEffectManager::CanBePhysicallyResisted(Effect->GetEffectData().Type))
			{
				int Chance = FMath::RandRange(0, 100);
				if(Chance <= TargetCharacter->GetPhysicalResistance())
				{
					//resisted effect
					ATimedEventManager::SpawnPopupText(EPopupTextType::Resisted, 0, 2, TargetCharacter->GetActorLocation(), TargetCharacter->GetWorld(), false);
					continue;
				}
			}
			else if(StatusEffectManager::CanBeMagicallyResisted(Effect->GetEffectData().Type))
			{
				int Chance = FMath::RandRange(0, 100);
				if(Chance <= TargetCharacter->GetMagicalResistance())
				{
					//resisted effect
					ATimedEventManager::SpawnPopupText(EPopupTextType::Resisted, 0, 2, TargetCharacter->GetActorLocation(), TargetCharacter->GetWorld(), false);
					continue;
				}
			}

			switch(Effect->GetEffectData().Target)
			{
				case ESkillTargetingType::Caster:
					
					break;

				case ESkillTargetingType::AllCharacters:
					
					Effect->ApplyEffectToCharacter(TargetCharacter);
					break;
				
				case ESkillTargetingType::Enemy:

					Effect->ApplyEffectToCharacter(TargetCharacter);
					break;

				case ESkillTargetingType::Friend:

					Effect->ApplyEffectToCharacter(TargetCharacter);
					break;
				
				default:

					Effect->ApplyEffectToCharacter(TargetCharacter);
					break;
			}
		}

	}
	
	for (auto CasterEffect : SkillEffects)
	{
		if(CasterEffect == nullptr || CasterEffect->GetEffectData().Target != ESkillTargetingType::Caster)
		{
			continue;
		}
		int EffectChance = FMath::RandRange(0,100);
		if(EffectChance > CasterEffect->GetEffectData().Chance)
		{
			//missed effect
			ATimedEventManager::SpawnPopupText(EPopupTextType::MissedEffect, 0, 2, Caster->GetActorLocation(), Caster->GetWorld(), false);
			continue;
		}

		CasterEffect->ApplyEffectToCharacter(Caster);
	}
}

FCharSkillFileDataTable* CharacterSkill::GetSkillFilesFromTable(int SkillID, UWorld* World)
{
 if(World == nullptr) return nullptr;
    
    UGuildGameInstance* GameInstance = Cast<UGuildGameInstance>(UGameplayStatics::GetGameInstance(World));

    if(GameInstance == nullptr || GameInstance->CharacterSkillsFileTable == nullptr)
    {
        return nullptr;
    }
     const FName RowName = *(FString::FromInt(SkillID));
     FCharSkillFileDataTable* SkillFiles = GameInstance->CharacterSkillsFileTable->FindRow<FCharSkillFileDataTable>(RowName, "Character Skill File Row Missing", true);
     
     return  SkillFiles;
}
