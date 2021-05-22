// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GGCharacterBase.generated.h"

UCLASS()
class GUILDGAME_API AGGCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGGCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* RWeaponStaticMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* LWeaponStaticMesh;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* RWeaponSkeletalMesh;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* LWeaponSkeletalMesh;

	bool bIsLWeaponSkeletal;
	bool bIsRWeaponSkeletal;

	template<typename T>
	void SetWeaponMesh(T* Mesh, bool bIsRightWeapon,bool bIsSkeletal);

	template<typename T>
	T* GetWeapon(bool bIsRightWeapon, bool& bOutIsSkeletal);
};

template <typename T>
void AGGCharacterBase::SetWeaponMesh(T* Mesh, bool bIsRightWeapon, bool bIsSkeletal)
{
	if(bIsRightWeapon)
	{
		if(bIsSkeletal)
		{
			RWeaponSkeletalMesh = Cast<USkeletalMeshComponent>(Mesh);
			bIsRWeaponSkeletal = true;
		}
		else
		{
			RWeaponStaticMesh = Cast<UStaticMeshComponent>(Mesh);
			bIsRWeaponSkeletal = false;
		}
	}
	else
	{
		if(bIsSkeletal)
		{
			LWeaponSkeletalMesh = Cast<USkeletalMeshComponent>(Mesh);
			bIsLWeaponSkeletal = true;
		}
		else
		{
			LWeaponStaticMesh = Cast<UStaticMeshComponent>(Mesh);
			bIsLWeaponSkeletal = false;
		}
	}
}

template <typename T>
T* AGGCharacterBase::GetWeapon(bool bIsRightWeapon, bool& bOutIsSkeletal)
{
	if(bIsRightWeapon)
	{
		if(bIsRWeaponSkeletal)
		{
			return RWeaponSkeletalMesh;
		}
		else
		{
			return RWeaponStaticMesh;
		}
	}
	else
	{
		if(bIsLWeaponSkeletal)
		{
			return LWeaponSkeletalMesh;
		}
		else
		{
			return LWeaponStaticMesh;
		}
	}
}
