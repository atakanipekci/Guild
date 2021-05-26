// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSkills.h"

#include "ShapeFactory.h"
#include "EffectFactory.h"

CharacterSkills::CharacterSkills(const FSkillData& Data)
{
	SkillData = Data;
	Shape = ShapeFactory::CreateShape(Data.ShapeType, Data.ShapeParameters);
	for (auto Element : Data.EffectData)
	{
		SkillEffects.Add(EffectFactory::CreateEffect(Element));		
	}
}

CharacterSkills::~CharacterSkills()
{
	delete Shape;
	for (auto Element : SkillEffects)
	{
		delete Element;
	}
}

FCharSkillFileDataTable* CharacterSkills::GetSkillFiles(int SkillID, UWorld* World)
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
