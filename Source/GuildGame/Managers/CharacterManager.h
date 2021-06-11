// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GuildGameInstance.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GuildGame/Characters/CharacterStats.h"
#include "GuildGame/Characters/GGCharacter.h"
#include "GuildGame/Characters/WeaponActor.h"
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
                    SkeletalMesh->SetAnimInstanceClass(CharFiles->AnimationBP);

                    AGGCharacterBase* SpawnedCharBase = Cast<AGGCharacterBase>(SpawnedCharacter);
                    if(SpawnedCharBase)
                    {
                        for (int i = 0; i < CharFiles->WeaponData.Num(); ++i)
                        {
                            FActorSpawnParameters SpawnParameters;
                            AWeaponActor* WeaponActor =  World->SpawnActor<AWeaponActor>(AWeaponActor::StaticClass(), SpawnParameters);
                            if(WeaponActor)
                            {
                                if(CharFiles->WeaponData[i].SkeletalMesh)
                                {
                                    USkeletalMeshComponent* WpSkeletal = WeaponActor->SkeletalMesh;
                                    if(WpSkeletal)
                                    {
                                        WpSkeletal->SetSkeletalMesh(CharFiles->WeaponData[i].SkeletalMesh);
                                        WpSkeletal->SetRelativeLocation(FVector::ZeroVector);
                                        WpSkeletal->SetRelativeRotation(FRotator::ZeroRotator);
                                        WpSkeletal->SetCollisionEnabled(ECollisionEnabled::NoCollision);
                                        // WpSkeletal->SetAnimInstanceClass(CharFiles->AnimationBP);
                                        WpSkeletal->SetMasterPoseComponent(SkeletalMesh);
                                    }
                                }
                                else if(CharFiles->WeaponData[i].StaticMesh)
                                {
                                    UStaticMeshComponent* WpStatic = WeaponActor->StaticMesh;
                                    if(WpStatic)
                                    {
                                        WpStatic->SetStaticMesh(CharFiles->WeaponData[i].StaticMesh);
                                        WpStatic->SetRelativeLocation(FVector::ZeroVector);
                                        WpStatic->SetRelativeRotation(FRotator::ZeroRotator);
                                        WpStatic->SetCollisionEnabled(ECollisionEnabled::NoCollision);
                                    }
                                }
                                WeaponActor->SetActorRelativeLocation(CharFiles->WeaponData[i].RelativeLocation);
                                WeaponActor->SetActorRelativeRotation(CharFiles->WeaponData[i].RelativeRotation);
                                WeaponActor->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::KeepRelativeTransform, CharFiles->WeaponData[i].BoneName);
                                
                                SpawnedCharBase->AddWeaponActor(WeaponActor);
                            }
                        }
                    }
                    
                }
            }       
        }
    }
    return SpawnedCharacter;
}
