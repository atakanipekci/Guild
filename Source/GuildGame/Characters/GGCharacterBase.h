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

	virtual void OnAttackHitsEnemies();
	virtual void OnAttackHitsEnemy(AActor* TargetToHit) ;
	virtual void OnDeath();
	virtual void OnCastingSkillEnds();
	virtual void ThrowProjectile(FName SocketName, bool bUseBoneRotation);
	virtual void CreateParticleOnTargetGrid() ;
	virtual void CreateParticlesOnEveryEnemyInDamageable() ;

	UPROPERTY()
	TArray<AActor*> Weapons;


	void AddWeaponActor(class AWeaponActor* NewWeapon);
};



