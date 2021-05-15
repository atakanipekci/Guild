#include "CharacterGenerationManager.h"

#include "WidgetManager.h"
#include "GuildGame/Town/Navigation/TownNpcCharacter.h"

UDataTable* CharacterGenerationManager::CharactersDataTable = nullptr;
TArray<FName> CharacterGenerationManager::OrderedRowNames;

FCharacterStats* CharacterGenerationManager::CreateRandomCharacter()
{
	const FString Context(TEXT("CONTEXT DATATABLE TEXT"));
	const FString CharacterType = OrderedRowNames[ FMath::RandRange(0, OrderedRowNames.Num() - 1)].ToString();

	const FName Row = *(CharacterType);

	if(CharactersDataTable)
	{
		FCharacterStats* CharacterData = CharactersDataTable->FindRow<FCharacterStats>(Row, Context, true);
		int UniqueID = WidgetManager::GetAndSetDraggableSpawnCount();
		FCharacterStats* CopyStruct = new FCharacterStats(*CharacterData);
		// CopyStruct->Price = 50;
		CopyStruct->MaxHealth += CopyStruct->MaxHealth * FMath::RandRange(0.0f, 0.20f);
		CopyStruct->CurrentHealth = CopyStruct->MaxHealth;
		CopyStruct->BaseDamage += CopyStruct->BaseDamage * FMath::RandRange(0.0f, 0.05f);
		CopyStruct->UniqueID = UniqueID;
		CopyStruct->TownNpcBehaviourState = ENpcBehaviourStates::WalkingAround;

		return  CopyStruct;
	}
	else
	{
		return  nullptr;
	}
}

void CharacterGenerationManager::SetCharactersTable(UDataTable* Table)
{
	 if(Table != nullptr)
    {
        CharactersDataTable = Table;
        CharactersDataTable->GetRowMap().GenerateKeyArray(OrderedRowNames);
    }
}
