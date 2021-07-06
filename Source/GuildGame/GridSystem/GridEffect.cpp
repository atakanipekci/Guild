#include "GridEffect.h"

#include "GuildGameInstance.h"
#include "GuildGame/Managers/StatusEffectManager.h"
#include "GuildGame/Managers/TimedEventManager.h"
#include "Kismet/GameplayStatics.h"

GridEffect::GridEffect(int Val, int RemainingTurn, AGGCharacter* Owner)
{
	this->Value = Val;
	this->RemainingTurn = RemainingTurn;
	this->Owner = Owner;
}

GridEffectBurn::GridEffectBurn(int Damage, int RemainingTurn, AGGCharacter* Owner): GridEffect(Damage, RemainingTurn, Owner)
{
	
}

bool GridEffectBurn::ApplyEffectToCharacter(AGGCharacter* Char)
{
	if(Char == nullptr)
	{
		return false;
	}

	if(StatusEffectManager::CanBePhysicallyResisted(EEffectType::GridStatusBurn))
	{
		int Chance = FMath::RandRange(0, 100);
		if(Chance <= Char->GetPhysicalResistance())
		{
			//resisted effect
			ATimedEventManager::SpawnPopupText(EPopupTextType::Resisted, 0, 2, Char->GetActorLocation(), Char->GetWorld(), false);
			return false;
		}

		Char->TakePhysicalDamage(Value, Owner);
	}
	else if(StatusEffectManager::CanBeMagicallyResisted(EEffectType::GridStatusBurn))
	{
		int Chance = FMath::RandRange(0, 100);
		if(Chance <= Char->GetPhysicalResistance())
		{
			//resisted effect
			ATimedEventManager::SpawnPopupText(EPopupTextType::Resisted, 0, 2, Char->GetActorLocation(), Char->GetWorld(), false);
			return false;
		}

		Char->TakeMagicalDamage(Value, Owner);
	}


	return true;
}

FGridStatusEffectFile* GridEffect::GetStatusEffectFile(EEffectType StatusType, UWorld* World)
{
	if(World == nullptr)
	{
		return nullptr;
	}
	UGuildGameInstance* GameInstance = Cast<UGuildGameInstance>(UGameplayStatics::GetGameInstance(World));

	if(GameInstance == nullptr || GameInstance->GridStatusEffectFileTable == nullptr)
	{
		return nullptr;
	}

	const FName RowName = *(GetStatusFileRowName(StatusType));
	FGridStatusEffectFile* StatusFile = GameInstance->GridStatusEffectFileTable->FindRow<FGridStatusEffectFile>(
		RowName, "Grid Status Data File Row Missing", true);

	return StatusFile;
}

FString GridEffect::GetStatusFileRowName(EEffectType StatusType)
{
	if(StatusType == EEffectType::GridStatusBurn)
    {
        return  FString(TEXT("GridStatusBurn"));
    }
   
    return FString(TEXT("EMPTY"));
}
