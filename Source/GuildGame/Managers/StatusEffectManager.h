#pragma once
#include "Engine/DataTable.h"

#include "StatusEffectManager.generated.h"


UENUM(Blueprintable)
enum class EStatusEffectType : uint8
{
	Bleed,
	Poison,
	Stun,
	Armor
};

USTRUCT()
struct FStatusEffectData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStatusEffectType Type;

	FText StatusName;

	FText StatusDesc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int RemainingTurns; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxValue;

	UPROPERTY()
	AActor* Caster;
};

class GUILDGAME_API StatusEffectManager
{
public:

	static void AddStatusEffect(class AGGCharacter* Target, class AGGCharacter* Caster, TArray<FStatusEffectData>* StatusEffects);

	static void InitStatus(class AGGCharacter* Target, FStatusEffectData* StatusEffect);
	static void StackStatus(class AGGCharacter* Target, FStatusEffectData* ExistingStatusEffect,FStatusEffectData* NewStatusEffect);
	static void ApplyOnTurnBegins(class AGGCharacter* Target, TMap<EStatusEffectType, struct FStatusEffectData>*);
	//static void ApplyOnTurnEnds(class AGGCharacter* Target, TMap<EStatusEffectType, struct FStatusEffectData>*);
	static void OnStatusEnd(class AGGCharacter* Target, FStatusEffectData* StatusEffect);
	
};
