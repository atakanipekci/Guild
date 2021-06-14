#include "StatusEffectManager.h"

#include "GuildGame/GuildGameGameModeBase.h"
#include "GuildGame/Characters/GGCharacter.h"
#include "Kismet/GameplayStatics.h"

void StatusEffectManager::AddStatusEffect(class AGGCharacter* Target, class AGGCharacter* Caster, TArray<FStatusEffectData>* StatusEffects)
{
	if(Target == nullptr || Caster == nullptr || StatusEffects == nullptr || StatusEffects->Num() <= 0) return;

	TMap<EStatusEffectType, struct FStatusEffectData>* AppliedEffects = Target->GetAppliedStatusEffects();

	if(AppliedEffects)
	{
		for (int i = 0; i < StatusEffects->Num(); ++i)
		{
			if(AppliedEffects->Contains((*StatusEffects)[i].Type))
			{
				(*StatusEffects)[i].Caster = Caster;
				FStatusEffectData* ExistingStatus = AppliedEffects->Find((*StatusEffects)[i].Type);
				StackStatus(Target, ExistingStatus,&(*StatusEffects)[i]);
			}
			else
			{
				(*StatusEffects)[i].Caster = Caster;
				InitStatus(Target, &(*StatusEffects)[i]);
				AppliedEffects->Add((*StatusEffects)[i].Type, (*StatusEffects)[i]);
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
	else if(StatusEffect->Type == EStatusEffectType::Armor)
	{
		
	}
}

void StatusEffectManager::StackStatus(AGGCharacter* Target, FStatusEffectData* ExistingStatusEffect,
	FStatusEffectData* NewStatusEffect)
{
	if(Target == nullptr || ExistingStatusEffect == nullptr || NewStatusEffect == nullptr) return;

	if(NewStatusEffect->Type == EStatusEffectType::Bleed)
	{
		
	}
	else if(NewStatusEffect->Type == EStatusEffectType::Poison)
	{
		
	}
	else if(NewStatusEffect->Type == EStatusEffectType::Stun)
	{
		
	}
	else if(NewStatusEffect->Type == EStatusEffectType::Armor)
	{
		
	}
}

void StatusEffectManager::ApplyOnTurnBegins(AGGCharacter* Target, TMap<EStatusEffectType, struct FStatusEffectData>* StatusEffects)
{
	if(Target == nullptr || StatusEffects == nullptr) return;

	TArray<FStatusEffectData*> StatusToRemove;

	bool bSkipTurn = false;
	for (auto It = StatusEffects->CreateIterator(); It; ++It)
	{
		if(It.Key() == EStatusEffectType::Bleed)
		{
			const int Result = FMath::RandRange(It.Value().MinValue, It.Value().MaxValue);
			Target->TakeDefaultDamage(Result, It.Value().Caster);
		}
		else if(It.Key() == EStatusEffectType::Poison)
		{
			const int Result = FMath::RandRange(It.Value().MinValue, It.Value().MaxValue);
			Target->TakeDefaultDamage(Result, It.Value().Caster);
		}
		else if(It.Key() == EStatusEffectType::Stun)
		{
			bSkipTurn = true;
		}
		else if(It.Key() == EStatusEffectType::Armor)
		{
			
		}

		It.Value().RemainingTurns--;
		if(It.Value().RemainingTurns <= 0)
		{
			StatusToRemove.Add(&(It.Value()));
		}
	}

	for (int i = 0; i < StatusToRemove.Num(); ++i)
	{
		if(StatusToRemove[i])
		{
			
			OnStatusEnd(Target, StatusToRemove[i]);
			StatusEffects->Remove(StatusToRemove[i]->Type);
		}
	}

	if(bSkipTurn)
	{
		AGuildGameGameModeBase* GameMode = Cast<AGuildGameGameModeBase>(UGameplayStatics::GetGameMode(Target));
		if(GameMode)
		{
			GameMode->Next();
		}
	}
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
// 		else if(It.Key() == EStatusEffectType::Armor)
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
	else if(StatusEffect->Type == EStatusEffectType::Armor)
	{
		
	}
}
