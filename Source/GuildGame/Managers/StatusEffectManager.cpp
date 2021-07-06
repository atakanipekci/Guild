#include "StatusEffectManager.h"

#include "GuildGameInstance.h"
#include "GuildGame/Characters/GGCharacter.h"
#include "GuildGame/Skills/SkillEffect.h"
#include "Kismet/GameplayStatics.h"

FStatusEffectData* StatusEffectManager::FindTypeInArray(TArray<FStatusEffectData>* Array, EEffectType Type)
{
	if(Array)
	{
		for (int i = 0; i < Array->Num(); ++i)
		{
			if((*Array)[i].Data.Type == Type)
			{
				return &(*Array)[i];  
			}
		}
	}

	return nullptr;
}

void StatusEffectManager::AddStatusEffect(class AGGCharacter* Target, class AGGCharacter* Caster, const FEffectData* EffectData)
{
	if(Target == nullptr || Caster == nullptr || EffectData == nullptr) return;

	TArray<struct FStatusEffectData>* AppliedEffects = Target->GetAppliedStatusEffects();

	if(AppliedEffects)
	{
		FStatusEffectData* ExistingStatus = FindTypeInArray(AppliedEffects, EffectData->Type);
		if(ExistingStatus != nullptr)
		{
			ExistingStatus->Caster = Caster;
			
			StackStatus(Target, ExistingStatus,EffectData);
			InitStatus(Target, EffectData);
		}
		else
		{
			FStatusEffectData NewEffect;
			NewEffect.Data = *EffectData;
			NewEffect.Caster = Caster;
			
			InitStatus(Target, &NewEffect.Data);
			AppliedEffects->Add(NewEffect);
		}
	}
}

void StatusEffectManager::InitStatus(AGGCharacter* Target, const FEffectData* StatusEffect)
{
	if(Target == nullptr || StatusEffect == nullptr) return;

	if(StatusEffect->Type == EEffectType::StatusBleed)
	{
		
	}
	else if(StatusEffect->Type == EEffectType::StatusPoison)
	{
		
	}
	else if(StatusEffect->Type == EEffectType::StatusStun)
	{
		Target->SetStunned(true);
	}
	else if(StatusEffect->Type == EEffectType::StatusHeal)
	{
		
	}
	else if(StatusEffect->Type == EEffectType::StatusBuff)
	{
		for (auto It = StatusEffect->StatusEffectStatsMap.CreateConstIterator(); It; ++It)
		{
			if(It.Key() == EStatusEffectStatsType::Speed)
			{
				float Value = FMath::Abs(It.Value().Value);
				Target->AddAppliedSpeed(Value);
			}
		}
	}
	else if(StatusEffect->Type == EEffectType::StatusDeBuff)
	{
		for (auto It = StatusEffect->StatusEffectStatsMap.CreateConstIterator(); It; ++It)
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
	const FEffectData* NewStatusEffect)
{
	if(Target == nullptr || ExistingStatusEffect == nullptr || NewStatusEffect == nullptr) return;

	// ExistingStatusEffect->Caster = NewStatusEffect->Caster;
	ExistingStatusEffect->Data.MaxValue += NewStatusEffect->MaxValue;
	ExistingStatusEffect->Data.RemainingTurns += NewStatusEffect->RemainingTurns;

	if(NewStatusEffect->Type == EEffectType::StatusBuff)
	{
		for (auto It = NewStatusEffect->StatusEffectStatsMap.CreateConstIterator(); It; ++It)
		{
			if(It.Key() == EStatusEffectStatsType::Speed)
			{
				FStatusEffectStatsData* ExistingSpeedStatData = ExistingStatusEffect->Data.StatusEffectStatsMap.Find(EStatusEffectStatsType::Speed);
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
					ExistingStatusEffect->Data.StatusEffectStatsMap.Add(EStatusEffectStatsType::Speed, It.Value());
				}
			}
		}
	}
	else if(NewStatusEffect->Type == EEffectType::StatusDeBuff)
	{
		for (auto It = NewStatusEffect->StatusEffectStatsMap.CreateConstIterator(); It; ++It)
		{
			if(It.Key() == EStatusEffectStatsType::Speed)
			{
				FStatusEffectStatsData* ExistingSpeedStatData = ExistingStatusEffect->Data.StatusEffectStatsMap.Find(EStatusEffectStatsType::Speed);
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
					ExistingStatusEffect->Data.StatusEffectStatsMap.Add(EStatusEffectStatsType::Speed, It.Value());
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
		if((*StatusEffects)[i].Data.Type == EEffectType::StatusBleed)
		{
			Target->TakeDefaultDamage((*StatusEffects)[i].Data.MaxValue, (*StatusEffects)[i].Caster);
		}
		else if((*StatusEffects)[i].Data.Type == EEffectType::StatusPoison)
		{
			Target->TakeDefaultDamage((*StatusEffects)[i].Data.MaxValue, (*StatusEffects)[i].Caster);
		}
		else if((*StatusEffects)[i].Data.Type == EEffectType::StatusStun)
		{
		}
		else if((*StatusEffects)[i].Data.Type == EEffectType::StatusHeal)
		{
			Target->Heal((*StatusEffects)[i].Data.MaxValue, (*StatusEffects)[i].Caster);
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
		if((*StatusEffects)[i].Data.Type == EEffectType::StatusBleed)
		{
		}
		else if((*StatusEffects)[i].Data.Type == EEffectType::StatusPoison)
		{
		}
		else if((*StatusEffects)[i].Data.Type == EEffectType::StatusStun)
		{
			
		}
		else if((*StatusEffects)[i].Data.Type == EEffectType::StatusHeal)
		{
		}
		

		(*StatusEffects)[i].Data.RemainingTurns--;
		if((*StatusEffects)[i].Data.RemainingTurns <= 0)
		{
			OnStatusEnd(Target, &(*StatusEffects)[i]);
			StatusEffects->RemoveAt(i);
		}
	}
}

void StatusEffectManager::OnStatusEnd(AGGCharacter* Target, FStatusEffectData* StatusEffect)
{
	if(Target == nullptr || StatusEffect == nullptr) return;

	if(StatusEffect->Data.Type == EEffectType::StatusBleed)
	{
		
	}
	else if(StatusEffect->Data.Type == EEffectType::StatusPoison)
	{
		
	}
	else if(StatusEffect->Data.Type == EEffectType::StatusStun)
	{
		Target->SetStunned(false);
	}
	else if(StatusEffect->Data.Type == EEffectType::StatusHeal)
	{
		
	}
	else if(StatusEffect->Data.Type == EEffectType::StatusBuff)
	{
		for (auto It = StatusEffect->Data.StatusEffectStatsMap.CreateIterator(); It; ++It)
		{
			if(It.Key() == EStatusEffectStatsType::Speed)
			{
				float Value = FMath::Abs(It.Value().Value);
				Target->AddAppliedSpeed(-Value);
			}
		}
	}
	else if(StatusEffect->Data.Type == EEffectType::StatusDeBuff)
	{
		for (auto It = StatusEffect->Data.StatusEffectStatsMap.CreateIterator(); It; ++It)
		{
			if(It.Key() == EStatusEffectStatsType::Speed)
			{
				float Value = FMath::Abs(It.Value().Value);
				Target->AddAppliedSpeed(Value);
			}
		}
	}
}

bool StatusEffectManager::CanBePhysicallyResisted(EEffectType StatusEffectType)
{
	if(StatusEffectType == EEffectType::StatusBleed || StatusEffectType == EEffectType::StatusStun || StatusEffectType == EEffectType::StatusPoison)
	{
		return true;
	}

	return false;
}

bool StatusEffectManager::CanBeMagicallyResisted(EEffectType StatusEffectType)
{
	if(StatusEffectType == EEffectType::StatusDeBuff || StatusEffectType == EEffectType::StatusBurn || StatusEffectType == EEffectType::GridStatusBurn)
	{
		return true;
	}

	return false;
}

FString StatusEffectManager::GetStatusFileRowName(EEffectType StatusType)
{
	if(StatusType == EEffectType::StatusBleed)
    {
        return  FString(TEXT("Bleed"));
    }
    else if(StatusType == EEffectType::StatusPoison)
    {
        return  FString(TEXT("Poison"));
    }
    else if(StatusType == EEffectType::StatusStun)
    {
        return  FString(TEXT("Stun"));
    }
    else if(StatusType == EEffectType::StatusHeal)
    {
    	return  FString(TEXT("Heal"));
    }
	else if(StatusType == EEffectType::StatusBuff)
    {
    	return  FString(TEXT("Buff"));
    }
	else if(StatusType == EEffectType::StatusDeBuff)
    {
    	return  FString(TEXT("DeBuff"));
    }
   
    return FString(TEXT("EMPTY"));
}

FStatusEffectFileDataTable* StatusEffectManager::GetStatusEffectFile(EEffectType StatusType, UWorld* World)
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