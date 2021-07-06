#pragma once
#include "Engine/DataTable.h"
#include "GuildGame/Skills/SkillEffect.h"

#include "StatusEffectManager.generated.h"


USTRUCT()
struct FStatusEffectData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FEffectData Data;
	
	UPROPERTY()
	class AGGCharacter* Caster;
};


USTRUCT()
struct FStatusEffectStatsFileData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStatusEffectStatsType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText StatDesc;
};

USTRUCT(BlueprintType)
struct FStatusEffectFileDataTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEffectType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText StatusName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText StatusDesc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture* Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EStatusEffectStatsType,  FStatusEffectStatsFileData> StatFiles;
};

class GUILDGAME_API StatusEffectManager
{
public:
	static FStatusEffectData* FindTypeInArray(TArray<FStatusEffectData>* Array, EEffectType Type);
	static void AddStatusEffect(class AGGCharacter* Target, class AGGCharacter* Caster, const FEffectData* EffectData);

	static void InitStatus(class AGGCharacter* Target, const FEffectData* StatusEffect);
	static void StackStatus(class AGGCharacter* Target, FStatusEffectData* ExistingStatusEffect, const FEffectData* NewStatusEffect);
	static void ApplyOnTurnBegins(class AGGCharacter* Target, TArray<struct FStatusEffectData>*);
	static void ApplyOnTurnEnds(AGGCharacter* Target, TArray<struct FStatusEffectData>* StatusEffects);
	static void OnStatusEnd(class AGGCharacter* Target, FStatusEffectData* StatusEffect);

	static bool CanBePhysicallyResisted(EEffectType StatusEffectType);
	static bool CanBeMagicallyResisted(EEffectType StatusEffectType);
	
	static FString GetStatusFileRowName(EEffectType StatusType);

	static FStatusEffectFileDataTable* GetStatusEffectFile(EEffectType StatusType, UWorld* World);
	
	
};
