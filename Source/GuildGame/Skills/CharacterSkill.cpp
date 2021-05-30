// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSkill.h"

#include "ShapeFactory.h"
#include "EffectFactory.h"
#include "GGLogHelper.h"
#include "GuildGameInstance.h"
#include "Kismet/GameplayStatics.h"

CharacterSkill::CharacterSkill(const FSkillData& Data, const FCharSkillFileDataTable& File)
{
	SkillData = Data;
	SkillFile = File;
	
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
