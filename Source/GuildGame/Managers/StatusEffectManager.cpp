#include "StatusEffectManager.h"

#include "GuildGameInstance.h"
#include "GuildGame/GuildGameGameModeBase.h"
#include "GuildGame/Characters/GGCharacter.h"
#include "Kismet/GameplayStatics.h"

FStatusEffectData* StatusEffectManager::FindTypeInArray(TArray<FStatusEffectData>* Array, EStatusEffectType Type)
{
	if(Array)
	{
		for (int i = 0; i < Array->Num(); ++i)
		{
			if((*Array)[i].Type == Type)
			{
				return &(*Array)[i];  
			}
		}
	}

	return nullptr;
}

void StatusEffectManager::AddStatusEffect(class AGGCharacter* Target, class AGGCharacter* Caster, TArray<FStatusEffectData>* StatusEffects)
{
	if(Target == nullptr || Caster == nullptr || StatusEffects == nullptr || StatusEffects->Num() <= 0) return;

	TArray<struct FStatusEffectData>* AppliedEffects = Target->GetAppliedStatusEffects();

	if(AppliedEffects)
	{
		for (int i = 0; i < StatusEffects->Num(); ++i)
		{
			FStatusEffectData* ExistingStatus = FindTypeInArray(AppliedEffects, (*StatusEffects)[i].Type);
			if(ExistingStatus != nullptr)
			{
				(*StatusEffects)[i].Caster = Caster;
				
				StackStatus(Target, ExistingStatus,&(*StatusEffects)[i]);
				InitStatus(Target, &(*StatusEffects)[i]);
			}
			else
			{
				(*StatusEffects)[i].Caster = Caster;
				InitStatus(Target, &(*StatusEffects)[i]);
				AppliedEffects->Add((*StatusEffects)[i]);
			}
		}
	}
}

void StatusEffectManager::InitStatus(AGGCharacter* Target, FStatusEffectData* StatusEffect)
{
	if(Target == nullptr || StatusEffect == nullptr) return;

	if(StatusEffect->Type == EStatusEffectType::Bleed)
	{
		
	}
	else if(StatusEffect->Type == EStatusEffectType::Poison)
	{
		
	}
	else if(StatusEffect->Type == EStatusEffectType::Stun)
	{
		
	}
	else if(StatusEffect->Type == EStatusEffectType::Heal)
	{
		
	}
}

void StatusEffectManager::StackStatus(AGGCharacter* Target, FStatusEffectData* ExistingStatusEffect,
	FStatusEffectData* NewStatusEffect)
{
	if(Target == nullptr || ExistingStatusEffect == nullptr || NewStatusEffect == nullptr) return;

	ExistingStatusEffect->Caster = NewStatusEffect->Caster;
	ExistingStatusEffect->Value += NewStatusEffect->Value;
	ExistingStatusEffect->RemainingTurns += NewStatusEffect->RemainingTurns;
}

void StatusEffectManager::ApplyOnTurnBegins(AGGCharacter* Target, TArray<struct FStatusEffectData>* StatusEffects)
{
	if(Target == nullptr || StatusEffects == nullptr) return;

	for (int i = StatusEffects->Num() - 1; i >= 0; --i)
	{
		if((*StatusEffects)[i].Type == EStatusEffectType::Bleed)
		{
			Target->TakeDefaultDamage((*StatusEffects)[i].Value, (*StatusEffects)[i].Caster);
		}
		else if((*StatusEffects)[i].Type == EStatusEffectType::Poison)
		{
			Target->TakeDefaultDamage((*StatusEffects)[i].Value, (*StatusEffects)[i].Caster);
		}
		else if((*StatusEffects)[i].Type == EStatusEffectType::Stun)
		{
			
		}
		else if((*StatusEffects)[i].Type == EStatusEffectType::Heal)
		{
			Target->Heal((*StatusEffects)[i].Value, (*StatusEffects)[i].Caster);
		}

		(*StatusEffects)[i].RemainingTurns--;
		if((*StatusEffects)[i].RemainingTurns <= 0)
		{
			OnStatusEnd(Target, &(*StatusEffects)[i]);
			StatusEffects->RemoveAt(i);
		}
	}

	// if(bSkipTurn)
	// {
	// 	AGuildGameGameModeBase* GameMode = Cast<AGuildGameGameModeBase>(UGameplayStatics::GetGameMode(Target));
	// 	if(GameMode)
	// 	{
	// 		GameMode->Next();
	// 	}
	// }
}

// void StatusEffectManager::ApplyOnTurnEnds(AGGCharacter* Target, TMap<EStatusEffectType, struct FStatusEffectData>* StatusEffects)
// {
// 	if(Target == nullptr || StatusEffects == nullptr) return;
//
// 	for (auto It = StatusEffects->CreateIterator(); It; ++It)
// 	{
// 		if(It.Key() == EStatusEffectType::Bleed)
// 		{
// 			
// 		}
// 		else if(It.Key() == EStatusEffectType::Poison)
// 		{
// 			
// 		}
// 		else if(It.Key() == EStatusEffectType::Stun)
// 		{
// 			
// 		}
// 		else if(It.Key() == EStatusEffectType::Heal)
// 		{
// 			
// 		}
// 	}
// }

void StatusEffectManager::OnStatusEnd(AGGCharacter* Target, FStatusEffectData* StatusEffect)
{
	if(Target == nullptr || StatusEffect == nullptr) return;

	if(StatusEffect->Type == EStatusEffectType::Bleed)
	{
		
	}
	else if(StatusEffect->Type == EStatusEffectType::Poison)
	{
		
	}
	else if(StatusEffect->Type == EStatusEffectType::Stun)
	{
		
	}
	else if(StatusEffect->Type == EStatusEffectType::Heal)
	{
		
	}
}

FString StatusEffectManager::GetStatusFileRowName(EStatusEffectType StatusType)
{
	if(StatusType == EStatusEffectType::Bleed)
    {
        return  FString(TEXT("Bleed"));
    }
    else if(StatusType == EStatusEffectType::Poison)
    {
        return  FString(TEXT("Poison"));
    }
    else if(StatusType == EStatusEffectType::Stun)
    {
        return  FString(TEXT("Stun"));
    }
    else if(StatusType == EStatusEffectType::Heal)
    {
    	return  FString(TEXT("Heal"));
    }
   
    return FString(TEXT("EMPTY"));
}

FStatusEffectFileDataTable* StatusEffectManager::GetStatusEffectFile(EStatusEffectType StatusType, UWorld* World)
{
	if(World == nullptr)
	{
		return nullptr;
	}
	UGuildGameInstance* GameInstance = Cast<UGuildGameInstance>(UGameplayStatics::GetGameInstance(World));

	if(GameInstance == nullptr || GameInstance->StatusEffectsFileTable == nullptr)
	{
		return nullptr;
	}

	const FName RowName = *(GetStatusFileRowName(StatusType));
	FStatusEffectFileDataTable* StatusFile = GameInstance->StatusEffectsFileTable->FindRow<FStatusEffectFileDataTable>(
		RowName, "Status Data File Row Missing", true);

	return StatusFile;
}
