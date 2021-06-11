#pragma once
#include "Engine/DataTable.h"
#include "CharacterStats.generated.h"

enum class ENpcBehaviourStates: uint8;

UENUM(Blueprintable)
enum class ECharacterClassType: uint8
{
	Knight,
	Mage,
	Archer
};

USTRUCT(BlueprintType)
struct FWeaponFileData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector RelativeLocation;
 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator RelativeRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BoneName;
	
};

 USTRUCT(BlueprintType)
struct FCharFileDataTable : public FTableRowBase
{
    GENERATED_BODY()
 	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharacterClassType CharacterType;
 	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
 	TArray<FWeaponFileData> WeaponData;

 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimationBP;

 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> TakeHitMontage;
 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> DeathMontage;

 	UAnimMontage* GetRandomTakeHitMontage()
 	{
 		if(TakeHitMontage.Num() > 0)
 		{
 			int Index = 0;
 			Index = FMath::RandRange(0, TakeHitMontage.Num() - 1);
 			return TakeHitMontage[Index];
 		}

 		return  nullptr;
 	}

 	UAnimMontage* GetRandomDeathMontage()
 	{
 		if(DeathMontage.Num() > 0)
 		{
 			int Index = 0;
 			Index = FMath::RandRange(0, DeathMontage.Num() - 1);
 			return DeathMontage[Index];
 		}

 		return  nullptr;
 	}
};

USTRUCT()
struct FCharacterStats : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharacterClassType ClassType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ClassName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxHealth = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BaseDamage = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Speed = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementRange = 850;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange = 150;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Price;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int> SkillIDs;

	int SpendableSkillPoints = 0;

	bool bIsOwned = false;
	
	int UniqueID;
	
	ENpcBehaviourStates TownNpcBehaviourState;

	FCharacterStats() = default;
	FCharacterStats(const FCharacterStats& StructForCopy) = default;
	/*explicit FCharacterStats(const FCharacterStats& StructForCopy)
	{
		ClassType = StructForCopy.ClassType;
		ClassName = StructForCopy.ClassName;
		CurrentHealth = StructForCopy.CurrentHealth;
		MaxHealth = StructForCopy.MaxHealth;
		BaseDamage = StructForCopy.BaseDamage;
		MovementRange = StructForCopy.MovementRange;
		AttackRange = StructForCopy.AttackRange;
		Price = StructForCopy.Price;
		TownNpcBehaviourState = StructForCopy.TownNpcBehaviourState;
		SkillIDs = StructForCopy.SkillIDs;
		SpendableSkillPoints = StructForCopy.SpendableSkillPoints;
		bIsOwned = StructForCopy.bIsOwned;
		
		if(StructForCopy.UniqueID >= 0)
		{
			UniqueID = StructForCopy.UniqueID;
		}
		else
		{
			
		}
	}*/

	 bool operator==(const FCharacterStats & OtherStats) const
     {
         return true;
     }
};