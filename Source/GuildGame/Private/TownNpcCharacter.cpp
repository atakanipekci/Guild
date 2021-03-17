// Fill out your copyright notice in the Description page of Project Settings.


#include "TownNpcCharacter.h"


#include "TownGameModeBase.h"
#include "TownNavNodeActor.h"
#include "TownNpcAIController.h"
#include "TownNpcManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATownNpcCharacter::ATownNpcCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MovementComponent = Cast<UCharacterMovementComponent>(GetComponentByClass(UCharacterMovementComponent::StaticClass()));
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
	ATownGameModeBase* GameMode = Cast<ATownGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if(GameMode)
	{
		if(GameMode->NpcManager)
		{
			NpcManager = GameMode->NpcManager;
		}
	}
}

void ATownNpcCharacter::MoveToLocation(FVector TargetLocation)
{
	UE_LOG(LogTemp, Warning, TEXT("MoveToLocation %s"), *(TargetLocation.ToString()));

	if( AIController)
	{
		AIController->MoveToLocation(TargetLocation);
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

