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
		Target->SetStunned(true);
	}
	else if(StatusEffect->Type == EStatusEffectType::Heal)
	{
		
	}
	else if(StatusEffect->Type == EStatusEffectType::Buff)
	{
		for (auto It = StatusEffect->StatsMap.CreateIterator(); It; ++It)
		{
			if(It.Key() == EStatusEffectStatsType::Speed)
			{
				float Value = FMath::Abs(It.Value().Value);
				Target->AddAppliedSpeed(Value);
			}
		}
	}
	else if(StatusEffect->Type == EStatusEffectType::DeBuff)
	{
		for (auto It = StatusEffect->StatsMap.CreateIterator(); It; ++It)
		{
			if(It.Key() == EStatusEffectStatsType::Speed)
			{
				float Value = FMath::Abs(It.Value().Value);
				Target->AddAppliedSpeed(-Value);
			}
		}
	}
}

void StatusEffectManager::StackStatus(AGGCharacter* Target, FStatusEffectData* ExistingStatusEffect,
	FStatusEffectData* NewStatusEffect)
{
	if(Target == nullptr || ExistingStatusEffect == nullptr || NewStatusEffect == nullptr) return;

	ExistingStatusEffect->Caster = NewStatusEffect->Caster;
	ExistingStatusEffect->Value += NewStatusEffect->Value;
	ExistingStatusEffect->RemainingTurns += NewStatusEffect->RemainingTurns;

	if(NewStatusEffect->Type == EStatusEffectType::Buff)
	{
		for (auto It = NewStatusEffect->StatsMap.CreateIterator(); It; ++It)
		{
			if(It.Key() == EStatusEffectStatsType::Speed)
			{
				FStatusEffectStatsData* ExistingSpeedStatData = ExistingStatusEffect->StatsMap.Find(EStatusEffectStatsType::Speed);
				if(ExistingSpeedStatData)
				{
					float Value = FMath::Abs(It.Value().Value);
					Target->AddAppliedSpeed(Value);
					ExistingSpeedStatData->Value += Value;
				}
				else if(ExistingSpeedStatData == nullptr)
				{
					float Value = FMath::Abs(It.Value().Value);
					Target->AddAppliedSpeed(Value);
					ExistingStatusEffect->StatsMap.Add(EStatusEffectStatsType::Speed, It.Value());
				}
			}
		}
	}
	else if(NewStatusEffect->Type == EStatusEffectType::DeBuff)
	{
		for (auto It = NewStatusEffect->StatsMap.CreateIterator(); It; ++It)
		{
			if(It.Key() == EStatusEffectStatsType::Speed)
			{
				FStatusEffectStatsData* ExistingSpeedStatData = ExistingStatusEffect->StatsMap.Find(EStatusEffectStatsType::Speed);
				if(ExistingSpeedStatData)
				{
					float Value = -FMath::Abs(It.Value().Value);
					Target->AddAppliedSpeed(Value);
					ExistingSpeedStatData->Value += Value;
				}
				else if(ExistingSpeedStatData == nullptr)
				{
					float Value = -FMath::Abs(It.Value().Value);
					Target->AddAppliedSpeed(Value);
					ExistingStatusEffect->StatsMap.Add(EStatusEffectStatsType::Speed, It.Value());
				}
			}
		}
	}
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

		

		// (*StatusEffects)[i].RemainingTurns--;
		// if((*StatusEffects)[i].RemainingTurns <= 0)
		// {
		// 	OnStatusEnd(Target, &(*StatusEffects)[i]);
		// 	StatusEffects->RemoveAt(i);
		// }
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

void StatusEffectManager::ApplyOnTurnEnds(AGGCharacter* Target, TArray<struct FStatusEffectData>* StatusEffects)
{
	if(Target == nullptr || StatusEffects == nullptr) return;

	for (int i = StatusEffects->Num() - 1; i >= 0; --i)
	{
		if((*StatusEffects)[i].Type == EStatusEffectType::Bleed)
		{
		}
		else if((*StatusEffects)[i].Type == EStatusEffectType::Poison)
		{
		}
		else if((*StatusEffects)[i].Type == EStatusEffectType::Stun)
		{
			
		}
		else if((*StatusEffects)[i].Type == EStatusEffectType::Heal)
		{
		}
		

		(*StatusEffects)[i].RemainingTurns--;
		if((*StatusEffects)[i].RemainingTurns <= 0)
		{
			OnStatusEnd(Target, &(*StatusEffects)[i]);
			StatusEffects->RemoveAt(i);
		}
	}
}

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
		Target->SetStunned(false);
	}
	else if(StatusEffect->Type == EStatusEffectType::Heal)
	{
		
	}
	else if(StatusEffect->Type == EStatusEffectType::Buff)
	{
		for (auto It = StatusEffect->StatsMap.CreateIterator(); It; ++It)
		{
			if(It.Key() == EStatusEffectStatsType::Speed)
			{
				float Value = FMath::Abs(It.Value().Value);
				Target->AddAppliedSpeed(-Value);
			}
		}
	}
	else if(StatusEffect->Type == EStatusEffectType::DeBuff)
	{
		for (auto It = StatusEffect->StatsMap.CreateIterator(); It; ++It)
		{
			if(It.Key() == EStatusEffectStatsType::Speed)
			{
				float Value = FMath::Abs(It.Value().Value);
				Target->AddAppliedSpeed(Value);
			}
		}
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
	else if(StatusType == EStatusEffectType::Buff)
    {
    	return  FString(TEXT("Buff"));
    }
	else if(StatusType == EStatusEffectType::DeBuff)
    {
    	return  FString(TEXT("DeBuff"));
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