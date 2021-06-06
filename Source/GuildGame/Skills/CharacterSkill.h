// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "SkillEffect.h"
#include "Engine/DataTable.h"
#include "SkillShape.h"
#include "CharacterSkill.generated.h"


enum class ESplineMeshType : uint8;


USTRUCT(BlueprintType)
struct FCharSkillFileDataTable : public FTableRowBase
{
    GENERATED_BODY()
 	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SkillID;
 	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* SkillImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* SkillMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> EffectBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Desc;

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
	float MinRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DiagonalRange = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ShowTrajectory = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillTargetingType TargetingType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELineOfSightType SkillLineOfSight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELineOfSightType EffectLineOfSight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.0", ClampMax = "80.0", UIMin = "0.0", UIMax = "80.0"))
	float LineOfSightAngle;

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
class GUILDGAME_API CharacterSkill
{
public:
	CharacterSkill(const FSkillData&, const FCharSkillFileDataTable&);
	~CharacterSkill();
	void ApplyEffects(AGGCharacter* Caster, TArray<AGGCharacter*>& TargetCharacters);
	FSkillData& GetSkillData()
	{
		return SkillData;
	}
	FCharSkillFileDataTable& GetSkillFiles()
	{
		return SkillFile;
	}

	SkillShape* GetShape() const
	{
		return Shape;
	}
	static FCharSkillFileDataTable* GetSkillFilesFromTable(int SkillID, UWorld* World);

private:
	FSkillData SkillData;
	FCharSkillFileDataTable SkillFile;
	SkillShape* Shape;
	TArray<SkillEffect*> SkillEffects;
};


