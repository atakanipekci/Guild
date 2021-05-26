// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "SkillEffect.h"
#include "Engine/DataTable.h"
#include "SkillShape.h"
#include "CharacterSkills.generated.h"


USTRUCT(BlueprintType)
struct FCharSkillFileDataTable : public FTableRowBase
{
    GENERATED_BODY()
 	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SkillID;
 	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* SkillImage;
};

USTRUCT()
struct FSkillData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SkillID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillTargetingType TargetingType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillShapeType ShapeType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> ShapeParameters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEffectData> EffectData;
};

/**
 * 
 */
class GUILDGAME_API CharacterSkills
{
public:
	CharacterSkills(const FSkillData&);
	~CharacterSkills();
	static FCharSkillFileDataTable* GetSkillFiles(int SkillID, UWorld* World);

private:
	FSkillData SkillData;
	SkillShape* Shape;
	TArray<SkillEffect*> SkillEffects;
};


