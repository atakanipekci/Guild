#pragma once
#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "GuildGame/Skills/SkillEffect.h"
#include "GuildGame/VFX/GridStatusEffectVfxActor.h"
#include "GridEffect.generated.h"


 USTRUCT()
 struct FGridStatusEffectFile : public FTableRowBase
 {
 	GENERATED_BODY()

 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
 	EEffectType Type;

 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
 	TSubclassOf<AGridStatusEffectVfxActor> VisualEffect;
 };


class GUILDGAME_API GridEffect
{
public:
	virtual bool ApplyEffectToCharacter(class AGGCharacter*) = 0;
	explicit GridEffect(int Val, int RemainingTurn, class AGGCharacter* Owner);
	virtual ~GridEffect() = default;

	int GetReminingTurn()
	{
		return  RemainingTurn;
	}

	int DecreaseRemainingTurn()
	{
		return --RemainingTurn;
	}

	static struct FGridStatusEffectFile* GetStatusEffectFile(EEffectType StatusType, UWorld* World);
	static FString GetStatusFileRowName(EEffectType StatusType);
protected:
	class AGGCharacter* Owner = nullptr;
	int Value = 0;
	int RemainingTurn = 0;
};

class GUILDGAME_API GridEffectBurn : public GridEffect
{
public:
	explicit GridEffectBurn(int Damage, int RemainingTurn, AGGCharacter* Owner);
	virtual bool ApplyEffectToCharacter(AGGCharacter*) override;

};


