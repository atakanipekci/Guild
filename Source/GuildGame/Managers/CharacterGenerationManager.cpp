#include "CharacterGenerationManager.h"

#include "GuildGameInstance.h"
#include "WidgetManager.h"
#include "GuildGame/Characters/CharacterStats.h"
#include "GuildGame/Town/Navigation/TownNpcCharacter.h"
#include "Kismet/GameplayStatics.h"

ACharacterGenerationManager* ACharacterGenerationManager::Instance = nullptr;

void ACharacterGenerationManager::BeginDestroy()
{
	Super::BeginDestroy();

	Instance = nullptr;
}

void ACharacterGenerationManager::CreateManagerInstance(UWorld* World)
{
	if(World == nullptr) return;
    
	if(Instance == nullptr)
	{
		Instance = World->SpawnActor<ACharacterGenerationManager>();

	    UGuildGameInstance* GameInstance = Cast<UGuildGameInstance>(UGameplayStatics::GetGameInstance(World));
	    if(GameInstance && Instance && GameInstance->CharacterStatsTable)
	    {
	        Instance->CharactersDataTable = GameInstance->CharacterStatsTable;
	    	Instance->CharactersDataTable->GetRowMap().GenerateKeyArray(Instance->OrderedRowNames);
	    }
	}
}

FCharacterStats* ACharacterGenerationManager::CreateRandomCharacter(UWorld* World)
{
	if(World == nullptr) return nullptr;
    
    if(Instance == nullptr)
    {
        CreateManagerInstance(World);
    }
	
    if(Instance  &&  Instance->CharactersDataTable)
    {
		const FString Context(TEXT("CONTEXT DATATABLE TEXT"));
		const FString CharacterType = Instance->OrderedRowNames[ FMath::RandRange(0, Instance->OrderedRowNames.Num() - 1)].ToString();

		const FName Row = *(CharacterType);

	
		FCharacterStats* CharacterData = Instance->CharactersDataTable->FindRow<FCharacterStats>(Row, Context, true);
		int UniqueID = AWidgetManager::IncrementSpawnedDraggableCount();
		FCharacterStats* CopyStruct = new FCharacterStats(*CharacterData);
		// CopyStruct->Price = 50;
		CopyStruct->MaxHealth = CopyStruct->MaxHealth + CopyStruct->MaxHealth * FMath::RandRange(0.0f, 0.20f);
		CopyStruct->CurrentHealth = CopyStruct->MaxHealth;

    	CopyStruct->MaxArmor = CopyStruct->MaxArmor + CopyStruct->MaxArmor * FMath::RandRange(0.0f, 0.20f);
		CopyStruct->CurrentArmor = CopyStruct->MaxArmor;

    	CopyStruct->MaxMagicArmor = CopyStruct->MaxMagicArmor + CopyStruct->MaxMagicArmor * FMath::RandRange(0.0f, 0.20f);
		CopyStruct->CurrentMagicArmor = CopyStruct->MaxMagicArmor;
    	
		CopyStruct->BaseDamage += CopyStruct->BaseDamage * FMath::RandRange(0.0f, 0.05f);
		CopyStruct->UniqueID = UniqueID;
		CopyStruct->TownNpcBehaviourState = ENpcBehaviourStates::WalkingAround;

		CopyStruct->SpendableSkillPoints = FMath::RandRange(0, 3);

		return  CopyStruct;
	}
	
	return  nullptr;
}
