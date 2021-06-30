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

UCLASS()
class GUILDGAME_API ACharacterGenerationManager : public AActor
{

	GENERATED_BODY()
	
public:

	virtual void BeginDestroy() override;

	static ACharacterGenerationManager* Instance;

	static void CreateManagerInstance(UWorld* World);
	static FCharacterStats* CreateRandomCharacter(UWorld* World);

	UPROPERTY()
	UDataTable* CharactersDataTable;
	
	TArray<FName> OrderedRowNames;

	
};
