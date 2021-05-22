// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GuildGame/Characters/GGCharacter.h"
#include "SkillEffect.generated.h"


UENUM()
enum class EEffectType: uint8
{
	DealDamage,
	Heal,
	Stun,
	ApplyStatus,
	Move
};

UENUM()
enum class ESkillTargetingType: uint8
{
	Caster,
	Enemy,
	Friend,
	AllCharacters,
	Grid
};

USTRUCT()
struct FEffectData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEffectType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillTargetingType Target;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Chance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxValue;
};


/**
 * 
 */
class GUILDGAME_API SkillEffect
{
public:
	virtual bool ApplyEffectToCharacter(AGGCharacter*) = 0;
	virtual ~SkillEffect() = default;
protected:
	FEffectData Data;
};

class GUILDGAME_API EffectDealDamage : public SkillEffect
{
public:
	explicit EffectDealDamage(const FEffectData&);
	virtual bool ApplyEffectToCharacter(AGGCharacter*) override;
};

class GUILDGAME_API EffectHeal : public SkillEffect
{
public:
	explicit EffectHeal(const FEffectData&);
	virtual bool ApplyEffectToCharacter(AGGCharacter*) override;
};
