// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GuildGame/Characters/GGCharacterBase.h"

#include "TownNpcCharacter.generated.h"

UENUM()
enum class ENpcBehaviourStates: uint8
{
	WalkingAround,
	AtTheTavern
};

UCLASS()
class GUILDGAME_API ATownNpcCharacter : public AGGCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATownNpcCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	class UCharacterAnimInstance* AnimInstance;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UCharacterAnimInstance* GetAnimInstance();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveToLocation(FVector TargetLocation);
	void MoveToRandomLocation();
	void SetBehaviourState(ENpcBehaviourStates State);

	UPROPERTY()
	class ATownNpcAIController* AIController;

	ENpcBehaviourStates CurrentBehaviourState;

	UPROPERTY()
	class ATownNavNodeActor* DestinationNode;

	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;


};
