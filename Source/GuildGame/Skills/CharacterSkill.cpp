// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSkill.h"

#include "ShapeFactory.h"
#include "EffectFactory.h"
#include "GGLogHelper.h"
#include "GuildGameInstance.h"
#include "GuildGame/GuildGameGameModeBase.h"
#include "GuildGame/Managers/StatusEffectManager.h"
#include "Kismet/GameplayStatics.h"

CharacterSkill::CharacterSkill(const FSkillData& Data, const FCharSkillFileDataTable& File)
{
	SkillData = Data;
	SkillFile = File;

	SkillID = SkillData.SkillID;
	
	Shape = ShapeFactory::CreateShape(Data.ShapeType, Data.ShapeParameters);
	for (auto Element : Data.EffectData)
	{
		SkillEffects.Add(EffectFactory::CreateEffect(Element));		
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
	//todo: add team controls
	for (auto Element : SkillEffects)
	{
		if(Element)
		{
			int Chance = FMath::RandRange(0,100);
			if(Chance > Element->GetEffectData().Chance)
			{
				continue;
			}
			switch(Element->GetEffectData().Target)
			{
				case ESkillTargetingType::Caster:
					Element->ApplyEffectToCharacter(Caster);
					break;

				case ESkillTargetingType::AllCharacters:
					for (auto TargetCharacter : TargetCharacters)
					{
						Element->ApplyEffectToCharacter(TargetCharacter);
					}
					break;
				case ESkillTargetingType::Enemy:
					for (auto TargetCharacter : TargetCharacters)
					{
						Element->ApplyEffectToCharacter(TargetCharacter);
					}
					break;

				case ESkillTargetingType::Friend:
					for (auto TargetCharacter : TargetCharacters)
					{
						Element->ApplyEffectToCharacter(TargetCharacter);
					}
					break;
				
				
				default:
					for (auto TargetCharacter : TargetCharacters)
					{
						Element->ApplyEffectToCharacter(TargetCharacter);
					}
					break;
			}
		}
	}
}

void CharacterSkill::ApplyStatus(AGGCharacter* Caster, TArray<AGGCharacter*>& TargetCharacters)
{
	if(Caster == nullptr) return;

	for (int i = 0; i < TargetCharacters.Num(); ++i)
	{
		if(TargetCharacters[i])
		{
			StatusEffectManager::AddStatusEffect(TargetCharacters[i], Caster, &SkillData.StatusEffects);
		}
	}

	AGuildGameGameModeBase* BattleGameMode = Cast<AGuildGameGameModeBase>(UGameplayStatics::GetGameMode(Caster));
	if(BattleGameMode)
	{
		BattleGameMode->BattleTurnManager.UpdateWidgetOrder(true);
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
