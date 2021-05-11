#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GuildGame/Town/Navigation/TownNpcManager.h"

#include "CharacterGenerationManager.generated.h"

UENUM()
enum class ECharacterTiers: uint8
{
	D,
	C,
	B,
	A,
	S
};

USTRUCT(BlueprintType)
struct FTierPercentages
{
    GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharacterTiers TierType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0", UIMax = "100.0"))
	float HpPercentage;
};

USTRUCT(BlueprintType)
struct FCharacterGenerationStats : public FTableRowBase
{
    GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharacterClassType ClassType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTierPercentages> Tiers;
};

class GUILDGAME_API CharacterGenerationManager
{
public:
	
};
