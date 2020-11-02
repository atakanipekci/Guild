// Fill out your copyright notice in the Description page of Project Settings.


#include "GGCharacter.h"


#include "AIController.h"
#include "GGAIController.h"
#include "GGPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterStatsComponent.h"
#include "GridFloor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGGCharacter::AGGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);
	StatsComponent = CreateDefaultSubobject<UCharacterStatsComponent>("StatsComponent");
	if(StatsComponent)
	{
		StatsComponent->SetMaxHealth(100);
		StatsComponent->SetCurrentHealth(100);
	}
}

TArray<GGGrid*>* AGGCharacter::GetMovableGrids()
{
	return &MovableGrids;
}

TArray<GGGrid*>* AGGCharacter::GetDamageableGrids()
{
	return &DamageableGrids;
}

// Called when the game starts or when spawned
void AGGCharacter::BeginPlay()
{
	Super::BeginPlay();
	/*AGGPlayerController* PlayerController = Cast<AGGPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController != nullptr)
	{
		PlayerController->SetSelectedCharacter(this);
	}*/
	
}

// Called every frame
void AGGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGGCharacter::MoveTo(FVector TargetPos)
{
	AGGAIController* AIController = Cast<AGGAIController>(GetController());
	if(AIController && Status == ECharacterStatus::Idle)
	{
		SetStatus(ECharacterStatus::Moving);
		GridManager* GridMan = CharacterManager::CharGridManager;
		if(GridMan && GridMan->GetAttachedFloor())
		{
			GridMan->GetAttachedFloor()->ClearGridMeshes();
			GridMan->GetAttachedFloor()->ClearPath();
		}
		AIController->MoveToLocation(TargetPos,5,false,true,false,true,0,false);
	}
}

void AGGCharacter::UpdateMovableGrids()
{
	CharacterManager::SetMovableGrids(this);
}

void AGGCharacter::UpdateDamageableGrids()
{
	CharacterManager::SetDamageableGrids(this);
}

void AGGCharacter::AttackTo(AGGCharacter* Target)
{
	if(Target == nullptr)
	{
		return;
	}

	if(!CharacterManager::CanAttackTo(this,Target))
	{
		return;
	}

	Target->TakeDefaultDamage(GetBaseDamage(),this);
}

void AGGCharacter::SetSelected()
{
	UpdateMovableGrids();
	GridManager* GridMan = CharacterManager::CharGridManager;
	if(GridMan && GridMan->GetAttachedFloor())
	{
		GridMan->GetAttachedFloor()->UpdateGridMeshes(MovableGrids);
	}
}

float AGGCharacter::GetDefaultMovementRange() const
{
	if(StatsComponent == nullptr)
	{
		return 0;
	}
	
	return StatsComponent->GetMovementRange();
}

float AGGCharacter::GetDefaultDamageRange() const
{
	if(StatsComponent == nullptr)
	{
		return 0;
	}
	
	return StatsComponent->GetAttackRange();
}

int AGGCharacter::GetBaseDamage() const
{
	if(StatsComponent == nullptr)
	{
		return 0;
	}
	
	return StatsComponent->GetBaseDamage();
}

ECharacterStatus AGGCharacter::GetStatus() const
{
	return Status;
}

void AGGCharacter::SetStatus(ECharacterStatus NewStatus)
{
	Status = NewStatus;
	if(Status == ECharacterStatus::Idle)
	{
		UpdateMovableGrids();
		GridManager* GridMan = CharacterManager::CharGridManager;
		if(GridMan && GridMan->GetAttachedFloor())
		{
			GridMan->GetAttachedFloor()->UpdateGridMeshes(MovableGrids);
		}
	}
}

float AGGCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCause)
{
	if(StatsComponent)
	{
		StatsComponent->ChangeHealth(-DamageAmount);
	}
	return DamageAmount;
}

float AGGCharacter::TakeDefaultDamage(float DamageAmount, AActor* Dealer)
{
	return TakeDamage(DamageAmount, FDamageEvent(), nullptr,this);
}

void AGGCharacter::ShowDamageableGrids()
{
	UpdateDamageableGrids();
	GridManager* GridMan = CharacterManager::CharGridManager;
	if(GridMan && GridMan->GetAttachedFloor())
	{
		GridMan->GetAttachedFloor()->UpdateGridMeshes(DamageableGrids, EISMType::Attack);
	}
	
}

