// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GuildGame/Characters/GGCharacter.h"
#include "SkillEffect.generated.h"


UENUM()
enum class EEffectType: uint8
{
	DealDamage,
	DealPhysicalDamage,
	DealMagicalDamage,
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

UENUM(Blueprintable)
enum class ELineOfSightType : uint8
{
	GoesThroughNothing,
	GoesThroughCharacters,
	GoesThroughEverything
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExtraValue;
};


/**
 * 
 */
class GUILDGAME_API SkillEffect
{
public:
	virtual bool ApplyEffectToCharacter(AGGCharacter*) = 0;
	virtual ~SkillEffect() = default;
	const FEffectData& GetEffectData() const
	{
		return Data;
	}
protected:
	FEffectData Data;
	AGGCharacter* Owner = nullptr;
};

class GUILDGAME_API EffectDealDamage : public SkillEffect
{
public:
	explicit EffectDealDamage(const FEffectData&, AGGCharacter* Owner);
	virtual bool ApplyEffectToCharacter(AGGCharacter*) override;
};

class GUILDGAME_API EffectDealPhysicalDamage : public SkillEffect
{
public:
	explicit EffectDealPhysicalDamage(const FEffectData&, AGGCharacter* Owner);
	virtual bool ApplyEffectToCharacter(AGGCharacter*) override;
};

class GUILDGAME_API EffectDealMagicalDamage : public SkillEffect
{
public:
	explicit EffectDealMagicalDamage(const FEffectData&, AGGCharacter* Owner);
	virtual bool ApplyEffectToCharacter(AGGCharacter*) override;
};

class GUILDGAME_API EffectHeal : public SkillEffect
{
public:
	explicit EffectHeal(const FEffectData& , AGGCharacter* OwnerChar);
	virtual bool ApplyEffectToCharacter(AGGCharacter*) override;
};
