#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "CharacterGenerationManager.generated.h"

struct FCharacterStats;
enum class ECharacterClassType : uint8;
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
	static FCharacterStats* CreateRandomCharacter();
	static void SetCharactersTable(UDataTable* Table);

	static UDataTable* CharactersDataTable;
	static TArray<FName> OrderedRowNames;

	
};
