// Fill out your copyright notice in the Description page of Project Settings.


#include "TownNpcCharacter.h"


#include "GuildGame/Town/TownGameModeBase.h"
#include "TownNavNodeActor.h"
#include "TownNpcAIController.h"
#include "TownNpcManager.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GuildGame/Characters/CharacterAnimInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATownNpcCharacter::ATownNpcCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MovementComponent = Cast<UCharacterMovementComponent>(GetComponentByClass(UCharacterMovementComponent::StaticClass()));

	// WeaponComponents.Add(CreateDefaultSubobject<UStaticMeshComponent>("RightWeapon"));
	// if(WeaponComponents.Num() >= 1 && WeaponComponents[0])
	// {
 //       WeaponComponents[0]->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
 //       WeaponComponents[0]->SetRelativeLocation(FVector::ZeroVector);
	// }
 //
	// WeaponComponents.Add(CreateDefaultSubobject<UStaticMeshComponent>("LeftWeapon"));
	// if(WeaponComponents.Num() >= 2 && WeaponComponents[1])
	// {
 //       WeaponComponents[1]->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
 //       WeaponComponents[1]->SetRelativeLocation(FVector::ZeroVector);
	// }
	
	
}

// Called when the game starts or when spawned
void ATownNpcCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if(MovementComponent)
	{
		MovementComponent->MaxWalkSpeed = FMath::RandRange(165, 185);
	}

	AIController = Cast<ATownNpcAIController>(GetController());

	// USkeletalMeshComponent* SkeletalMesh = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	// if(WeaponComponents[0] && SkeletalMesh)
	// {
 //        WeaponComponents[0]->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("hand_r"));
	// }
	// if(WeaponComponents[1] && SkeletalMesh)
	// {
 //        WeaponComponents[1]->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("hand_l"));
	// }

	if(AnimInstance == nullptr)
	{
		USkeletalMeshComponent* Skeletal = GetMesh();
		if(Skeletal)
		{
			AnimInstance = Cast<UCharacterAnimInstance>(Skeletal->GetAnimInstance());

			if(AnimInstance)
			{
				AnimInstance->SetOwnerCharacter(this);
			}
		}
	}

}

void ATownNpcCharacter::MoveToLocation(FVector TargetLocation)
{
	UE_LOG(LogTemp, Warning, TEXT("MoveToLocation %s"), *(TargetLocation.ToString()));

	if( AIController)
	{
		AIController->MoveToLocation(TargetLocation);

		SetAnimState(ECharacterAnimState::Run);
	//	UE_LOG(LogTemp, Warning, TEXT("IS FOLLIWIGN A PATH == %d "), AIController->IsFollowingAPath());
	}
}

void ATownNpcCharacter::MoveToRandomLocation()
{
	UE_LOG(LogTemp, Warning, TEXT("MoveToRandomLocation"));
	if(DestinationNode)
	{
		if(DestinationNode->AvailableNodes.Num() > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("GETRANDOMNODE"));
			const int RandIndex = FMath::RandRange(0, DestinationNode->AvailableNodes.Num() - 1);
			DestinationNode = DestinationNode->AvailableNodes[RandIndex];

			if(DestinationNode)
			{
				MoveToLocation(DestinationNode->Destination);
			}
		}
	}
}

void ATownNpcCharacter::SetBehaviourState(ENpcBehaviourStates State)
{
}

void ATownNpcCharacter::SetAnimState(ECharacterAnimState AnimState)
{
	if(AnimInstance)
	{
		AnimInstance->ChangeAnimState(AnimState);
	}
}

// Called every frame
void ATownNpcCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATownNpcCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UCharacterAnimInstance* ATownNpcCharacter::GetAnimInstance()
{
	return  AnimInstance;
}

