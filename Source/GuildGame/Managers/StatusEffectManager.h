#pragma once
#include "Engine/DataTable.h"
#include "StatusEffectManager.generated.h"


UENUM(Blueprintable)
enum class EStatusEffectType : uint8
{
	Bleed,
	Poison,
	Stun,
	Heal,
	Buff,
	DeBuff
};

UENUM(Blueprintable)
enum class EStatusEffectStatsType : uint8
{
	Speed
};

USTRUCT()
struct FStatusEffectStatsData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value;
};

USTRUCT()
struct FStatusEffectData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStatusEffectType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EStatusEffectStatsType, FStatusEffectStatsData> StatsMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int RemainingTurns; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value;
	

	UPROPERTY()
	class AGGCharacter* Caster;
};

USTRUCT(BlueprintType)
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
	EStatusEffectType Type;

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
	static FStatusEffectData* FindTypeInArray(TArray<FStatusEffectData>* Array, EStatusEffectType Type);
	static void AddStatusEffect(class AGGCharacter* Target, class AGGCharacter* Caster, TArray<FStatusEffectData>* StatusEffects);

	static void InitStatus(class AGGCharacter* Target, FStatusEffectData* StatusEffect);
	static void StackStatus(class AGGCharacter* Target, FStatusEffectData* ExistingStatusEffect,FStatusEffectData* NewStatusEffect);
	static void StackStats(class AGGCharacter* Target, FStatusEffectData* ExistingStatusEffect,FStatusEffectData* NewStatusEffect);
	static void ApplyOnTurnBegins(class AGGCharacter* Target, TArray<struct FStatusEffectData>*);
	static void ApplyOnTurnEnds(AGGCharacter* Target, TArray<struct FStatusEffectData>* StatusEffects);
	static void OnStatusEnd(class AGGCharacter* Target, FStatusEffectData* StatusEffect);
	
	static FString GetStatusFileRowName(EStatusEffectType StatusType);

	static FStatusEffectFileDataTable* GetStatusEffectFile(EStatusEffectType StatusType, UWorld* World);
	
	
};
