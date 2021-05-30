#pragma once
#include "Engine/DataTable.h"
#include "CharacterStats.generated.h"

enum class ENpcBehaviourStates: uint8;

UENUM()
enum class ECharacterClassType: uint8
{
	Knight,
	Mage
};

USTRUCT(BlueprintType)
struct FWeaponFileData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* RightHandWeaponStaticMesh;

 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* LeftHandWeaponStaticMesh;

 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* RightHandWeaponSkeletalMesh;

 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* LeftHandWeaponSkeletalMesh;

 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector RelativeLocationLeftHandWeapon;
 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator RelativeRotationLeftHandWeapon;

 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector RelativeLocationRightHandWeapon;
 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator RelativeRotationRightHandWeapon;
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
 	FWeaponFileData WeaponData;

 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimationBP;

 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* GetHitMontage;
 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* DeathMontage;
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
	
	explicit FCharacterStats(const FCharacterStats& StructForCopy)
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
	}

	 bool operator==(const FCharacterStats & OtherStats) const
     {
         return true;
     }
};