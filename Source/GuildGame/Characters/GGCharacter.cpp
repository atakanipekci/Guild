// Fill out your copyright notice in the Description page of Project Settings.


#include "GGCharacter.h"


#include "AIController.h"
#include "CharacterAnimInstance.h"
#include "GuildGame/Managers/CharacterManager.h"
#include "GuildGame/Battle/BattleAIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterStatsComponent.h"
#include "GuildGameInstance.h"
#include "Components/WidgetComponent.h"
#include "GuildGame/GridSystem/GridFloor.h"
#include "GuildGame/Managers/TimedEventManager.h"
#include "GuildGame/Skills/CharacterSkill.h"
#include "GuildGame/Widgets/BattleHealthBarWidget.h"

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

	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>("HealthBarComponent");
	if(HealthBarComponent)
	{
		HealthBarComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	}

	GridManager* GridMan = CharacterManager::CharGridManager;
    if(GridMan)
    {
     	//CurrentGridIndex = GridMan->WorldToGrid(GetActorLocation());
    }

	
}

AGGCharacter::~AGGCharacter()
{
	for (auto Element : Skills)
	{
		delete Element;
	}
}

TArray<Grid*>* AGGCharacter::GetMovableGrids()
{
	return &MovableGrids;
}

TArray<Grid*>* AGGCharacter::GetTargetableGrids()
{
	return &TargetableGrids;
}

TArray<Grid*>* AGGCharacter::GetDamageableGrids()
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
	
	if(HealthBarComponent)
	{
		HealthBarWidget = Cast<UBattleHealthBarWidget>(HealthBarComponent->GetUserWidgetObject());
		// if(HealthBarWidget)
		// 	HealthBarWidget->SetRenderScale(FVector2D(0.1f, 0.1));
	}
}

// Called every frame
void AGGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGGCharacter::SetStats(const FCharacterStats& Stats)
{
	if(StatsComponent)
	{
		StatsComponent->SetStats(Stats);
		UpdateHealthBar();
		UGuildGameInstance* GameInstance = Cast<UGuildGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		for (auto Element : StatsComponent->GetSkillIDs())
		{
    		if(GameInstance != nullptr && GameInstance->CharacterSkillsTable != nullptr)
			{
    			FString Name;
    			Name.AppendInt(Element);
    	 		FSkillData* SkillData = GameInstance->CharacterSkillsTable->FindRow<FSkillData>(FName(Name),"Skill File Row Missing", true);

				if(SkillData)
				{
    				FCharSkillFileDataTable* SkillFile = CharacterSkill::GetSkillFilesFromTable(SkillData->SkillID, GetWorld());
					if(SkillFile)
					{
						Skills.Add(new CharacterSkill(*SkillData, *SkillFile));
					}
				}
			}
		}
	}
}

void AGGCharacter::SetFile(const FCharFileDataTable& File)
{
	CharFile = File;
}

void AGGCharacter::MoveTo(FVector TargetPos)
{
	ABattleAIController* AIController = Cast<ABattleAIController>(GetController());
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
		SetAnimState(ECharacterAnimState::Run);
	}
}

void AGGCharacter::UpdateMovableGrids()
{
	CharacterManager::SetMovableGrids(this);
}

void AGGCharacter::UpdateTargetableGrids(const FSkillData* SkillData)
{
	CharacterManager::SetTargetableGrids(this, SkillData);
}

void AGGCharacter::UpdateDamageableGrids(const CharacterSkill* Skill, int CenterIndex)
{
	CharacterManager::SetDamageableGrids(this, Skill, CenterIndex);
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
	GridManager* GridMan = CharacterManager::CharGridManager;
	if(GridMan && GridMan->GetAttachedFloor())
	{
		GridMan->SetGridState(CurrentGridIndex, EGridState::Empty);
		UpdateMovableGrids();
		GridMan->GetAttachedFloor()->UpdateGridMeshes(MovableGrids);
	}
}

void AGGCharacter::Deselect()
{
	GridManager* GridMan = CharacterManager::CharGridManager;
	if(GridMan && GridMan->GetAttachedFloor())
	{
		GridMan->SetGridState(CurrentGridIndex, EGridState::Obstacle);
	}
}

void AGGCharacter::SetCurrentIndex(int NewIndex)
{
	CurrentGridIndex = NewIndex;
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
	GridManager* GridMan = CharacterManager::CharGridManager;
	if(Status == ECharacterStatus::Idle)
	{
		UpdateMovableGrids();
		if(GridMan && GridMan->GetAttachedFloor())
		{
			GridMan->GetAttachedFloor()->UpdateGridMeshes(MovableGrids);
		}
		
     	CurrentGridIndex = GridMan->WorldToGrid(GetActorLocation());
	}
}

float AGGCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCause)
{
	if(StatsComponent)
	{
		bool bIsDead = StatsComponent->ChangeHealth(-DamageAmount);
		if(bIsDead == false)
		{
			PlayCharacterMontage(CharFile.GetRandomTakeHitMontage());
		}
		UpdateHealthBar();
	}
	return DamageAmount;
}

float AGGCharacter::TakeDefaultDamage(float DamageAmount, AActor* Dealer)
{
	return TakeDamage(DamageAmount, FDamageEvent(), nullptr,this);
}

float AGGCharacter::Heal(float HealAmount, AGGCharacter* Healer)
{
	if(StatsComponent)
	{
		StatsComponent->ChangeHealth(HealAmount);
		UpdateHealthBar();
	}
	return HealAmount;
}

void AGGCharacter::ShowTargetableGrids()
{
	UpdateTargetableGrids(&Skills[CurrentSkillIndex]->GetSkillData());
	GridManager* GridMan = CharacterManager::CharGridManager;
	if(GridMan && GridMan->GetAttachedFloor())
	{
		GridMan->GetAttachedFloor()->ClearGridMesh(EISMType::Target);
		GridMan->GetAttachedFloor()->UpdateGridMeshes(TargetableGrids, EISMType::Target);
	}
}

void AGGCharacter::ShowDamageableGrids(int CenterIndex, bool CreateNew)
{	
	UpdateDamageableGrids(Skills[CurrentSkillIndex], CenterIndex);
	
	GridManager* GridMan = CharacterManager::CharGridManager;
	if(GridMan && GridMan->GetAttachedFloor() && CreateNew)
	{
		GridMan->GetAttachedFloor()->ClearGridMesh(EISMType::Damage);
		GridMan->GetAttachedFloor()->UpdateGridMeshes(DamageableGrids, EISMType::Damage, false);
	}
	else if(CreateNew == false && GridMan && GridMan->GetAttachedFloor())
	{
		FVector PosDif = GridMan->GetGridCenter(CenterIndex) - GridMan->GetGridCenter(CurrentTargetGridIndex);
		GridMan->GetAttachedFloor()->SetProcMeshPosition(EISMType::Damage, PosDif);
	}
	CurrentTargetGridIndex = CenterIndex;
}

void AGGCharacter::CastSkill(TArray<AGGCharacter*>& TargetCharacters)
{
	if(CurrentSkillIndex >= Skills.Num() || CurrentSkillIndex < 0 || Skills[CurrentSkillIndex] == nullptr)
	{
		return;
	}

	SelectedTargetCharacters = TargetCharacters;
	
	FCharSkillFileDataTable* SkillFiles = &(Skills[CurrentSkillIndex]->GetSkillFiles());
	if(SkillFiles )
	{
		if(TargetCharacters.Num() > 0)
		{
			FVector Dir =  TargetCharacters[0]->GetActorLocation() - GetActorLocation();
			Dir.Z = 0;
			FRotator Rot = FRotationMatrix::MakeFromX(Dir).Rotator();

			ATimedEventManager::Rotate(this, Rot, 0.2f, GetWorld());

			//SetActorRotation(Rot);
		}
		
		PlayCharacterMontage(SkillFiles->SkillMontage);
	}
}

void AGGCharacter::OnAttackHitsEnemy()
{
	Super::OnAttackHitsEnemy();

	if(CurrentSkillIndex >= Skills.Num()|| CurrentSkillIndex < 0 || Skills[CurrentSkillIndex] == nullptr)
	{
		return;
	}

	if(SelectedTargetCharacters.Num() > 0)
	{
		Skills[CurrentSkillIndex]->ApplyEffects(this, SelectedTargetCharacters);
	}
}

void AGGCharacter::OnDeath()
{
	Super::OnDeath();

	PlayCharacterMontage(CharFile.GetRandomDeathMontage());
}

void AGGCharacter::UpdateHealthBar()
{
	if(HealthBarWidget && StatsComponent)
	{
		HealthBarWidget->SetHpBar(StatsComponent->GetCurrentHealth(), StatsComponent->GetMaxHealth());
	}
}

UCharacterAnimInstance* AGGCharacter::GetAnimInstance()
{
	return  AnimInstance;
}

void AGGCharacter::PrepareAnimInstance()
{
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

void AGGCharacter::SetAnimState(ECharacterAnimState AnimState)
{
	if(AnimInstance)
	{
		AnimInstance->ChangeAnimState(AnimState);
	}
}

void AGGCharacter::PlayCharacterMontage(UAnimMontage* Montage)
{
	UE_LOG(LogTemp, Warning, TEXT("PlayCharacterMontage1"));
	if(AnimInstance == nullptr) return;;

	UE_LOG(LogTemp, Warning, TEXT("PlayCharacterMontage2"));
	AnimInstance->PlayMontage(Montage);
}

