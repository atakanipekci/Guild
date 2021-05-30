// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GuildGameInstance.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GuildGame/Characters/CharacterStats.h"
#include "GuildGame/Characters/GGCharacter.h"
#include "Kismet/GameplayStatics.h"

enum class ECharacterClassType: uint8;

/**
 * 
 */
class GUILDGAME_API CharacterManager
{
public:
    static void SetMovableGrids(class AGGCharacter* Character);
    static void SetTargetableGrids(class AGGCharacter* Character, const struct FSkillData* SkillData);
    static void SetDamageableGrids(class AGGCharacter* Character, const class CharacterSkill*, int Index);
    static bool CanAttackTo(const class AGGCharacter*, const class AGGCharacter* );
    static class GridManager* CharGridManager;

	template<typename T, typename E>
	static T* SpawnCharacter(TSubclassOf<E> BPClass, ECharacterClassType CharacterType, FVector Location, FRotator Rotation, UWorld* World);

	static FString GetCharFileRowName(ECharacterClassType CharacterType);

	
};

template <typename T, typename E>
T* CharacterManager::SpawnCharacter(TSubclassOf<E> BPClass, ECharacterClassType CharacterType, FVector Location,
	FRotator Rotation, UWorld* World)
{
	if(World == nullptr)
    {
        return nullptr;
    }
    UGuildGameInstance* GameInstance = Cast<UGuildGameInstance>(UGameplayStatics::GetGameInstance(World));

    if(GameInstance == nullptr || GameInstance->CharacterFileTable == nullptr)
    {
        return nullptr;
    }
    T* SpawnedCharacter =  World->SpawnActor<T>(BPClass, Location, Rotation);
    if(SpawnedCharacter)
    {
        USkeletalMeshComponent* SkeletalMesh = Cast<USkeletalMeshComponent>(SpawnedCharacter->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
            
        if(SkeletalMesh)
        {
            const FName RowName = *(GetCharFileRowName(CharacterType));
            FCharFileDataTable* CharFiles = GameInstance->CharacterFileTable->FindRow<FCharFileDataTable>(RowName, "Character File Row Missing", true);

            if(CharFiles)
            {
                if(CharFiles->SkeletalMesh)
                {
                    SkeletalMesh->SetSkeletalMesh(CharFiles->SkeletalMesh);
        
                    if(CharFiles->AnimationBP)
                    {
                        SkeletalMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
                        SkeletalMesh->SetAnimInstanceClass(CharFiles->AnimationBP);
                    }


                    AGGCharacter* SpawnedCharBase = Cast<AGGCharacter>(SpawnedCharacter);
                    if(CharFiles->WeaponData.RightHandWeaponStaticMesh)
                    {
                        UStaticMeshComponent* WeaponMesh = NewObject<UStaticMeshComponent>(SkeletalMesh,UStaticMeshComponent::StaticClass(), TEXT("RightHandWeapon"));
                        WeaponMesh->SetStaticMesh(CharFiles->WeaponData.RightHandWeaponStaticMesh);
                        WeaponMesh->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("hand_r"));
                        WeaponMesh->RegisterComponent();
                        WeaponMesh->SetRelativeLocation(CharFiles->WeaponData.RelativeLocationRightHandWeapon);
                        WeaponMesh->SetRelativeRotation(CharFiles->WeaponData.RelativeRotationRightHandWeapon);
                        WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


                        if(SpawnedCharBase) SpawnedCharBase->SetWeaponMesh(WeaponMesh, true, false);
                    }
                    if(CharFiles->WeaponData.RightHandWeaponSkeletalMesh)
                    {
                        USkeletalMeshComponent* WeaponMesh = NewObject<USkeletalMeshComponent>(SkeletalMesh,USkeletalMeshComponent::StaticClass(), TEXT("RightHandWeaponSkeletal"));
                        WeaponMesh->SetSkeletalMesh(CharFiles->WeaponData.RightHandWeaponSkeletalMesh);
                        WeaponMesh->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("hand_r"));
                        WeaponMesh->RegisterComponent();
                        WeaponMesh->SetRelativeLocation(CharFiles->WeaponData.RelativeLocationRightHandWeapon);
                        WeaponMesh->SetRelativeRotation(CharFiles->WeaponData.RelativeRotationRightHandWeapon);
                        WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

                        if(SpawnedCharBase) SpawnedCharBase->SetWeaponMesh(WeaponMesh, true, true);
                    }
                    
                    if(CharFiles->WeaponData.LeftHandWeaponStaticMesh)
                    {
                        UStaticMeshComponent* WeaponMesh = NewObject<UStaticMeshComponent>(SkeletalMesh,UStaticMeshComponent::StaticClass(), TEXT("LeftHandWeapon"));
                        WeaponMesh->SetStaticMesh(CharFiles->WeaponData.LeftHandWeaponStaticMesh);
                        WeaponMesh->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("hand_l"));
                        WeaponMesh->RegisterComponent();
                        WeaponMesh->SetRelativeLocation(CharFiles->WeaponData.RelativeLocationLeftHandWeapon);
                        WeaponMesh->SetRelativeRotation(CharFiles->WeaponData.RelativeRotationLeftHandWeapon);
                        WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

                        if(SpawnedCharBase) SpawnedCharBase->SetWeaponMesh(WeaponMesh, false, false);
                    }

                    if(CharFiles->WeaponData.LeftHandWeaponSkeletalMesh)
                    {
                        USkeletalMeshComponent* WeaponMesh = NewObject<USkeletalMeshComponent>(SkeletalMesh,USkeletalMeshComponent::StaticClass(), TEXT("LeftHandWeaponSkeletal"));
                        WeaponMesh->SetSkeletalMesh(CharFiles->WeaponData.LeftHandWeaponSkeletalMesh);
                        WeaponMesh->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("hand_l"));
                        WeaponMesh->RegisterComponent();
                        WeaponMesh->SetRelativeLocation(CharFiles->WeaponData.RelativeLocationLeftHandWeapon);
                        WeaponMesh->SetRelativeRotation(CharFiles->WeaponData.RelativeRotationRightHandWeapon);
                        WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

                        if(SpawnedCharBase) SpawnedCharBase->SetWeaponMesh(WeaponMesh, false, true);
                    }
                    
                }
            }       
        }
    }
    return SpawnedCharacter;
}
