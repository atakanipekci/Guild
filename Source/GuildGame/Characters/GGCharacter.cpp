// Fill out your copyright notice in the Description page of Project Settings.


#include "GGCharacter.h"


#include "AIController.h"
#include "CharacterAnimInstance.h"
#include "GuildGame/Managers/CharacterManager.h"
#include "GuildGame/Battle/BattleAIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterStatsComponent.h"
#include "GGLogHelper.h"
#include "GuildGameInstance.h"
#include "WeaponAnimInstance.h"
#include "Components/WidgetComponent.h"
#include "GuildGame/Battle/BattlePlayerController.h"
#include "GuildGame/GridSystem/GridFloor.h"
#include "GuildGame/Managers/TimedEventManager.h"
#include "GuildGame/Skills/CharacterSkill.h"
#include "GuildGame/VFX/Projectiles/Projectile.h"
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

	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
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
	
	// for (auto It = SkillsCooldownMap.CreateIterator(); It; ++It)
	// {
	// 	It.Value().RunClockAFrame(DeltaTime);
	// }

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

						FCooldownTimer CooldownData(SkillData->Cooldown);
						SkillsCooldownMap.Add(SkillData->SkillID, CooldownData);
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
		AIController->MoveToLocation(TargetPos,5,false,true,false,true,nullptr,false);
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

int AGGCharacter::GetSpeed() const
{
	if(StatsComponent == nullptr)
	{
		return 0;
	}
	
	return StatsComponent->GetSpeed();
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
	LOG("Status = %d", NewStatus);
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

	if(Status == ECharacterStatus::Moving || Status == ECharacterStatus::Casting)
	{
	LOG("22Status = %d", NewStatus);
		if(GridMan && GridMan->GetAttachedFloor())
		{
			GridMan->GetAttachedFloor()->ClearGridMeshes();
		}
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
	int Count = DamageableGrids.Num();
	UpdateDamageableGrids(Skills[CurrentSkillIndex], CenterIndex);
	int NewCount = DamageableGrids.Num();

	CreateNew = true;
	
	GridManager* GridMan = CharacterManager::CharGridManager;
	if(GridMan && GridMan->GetAttachedFloor() && CreateNew)
	{
		GridMan->GetAttachedFloor()->ClearGridMesh(EISMType::Damage);
		GridMan->GetAttachedFloor()->ClearGridMesh(EISMType::Target);
		GridMan->GetAttachedFloor()->UpdateGridMeshes(TargetableGrids, EISMType::Target, false, &DamageableGrids);
		GridMan->GetAttachedFloor()->UpdateGridMeshes(DamageableGrids, EISMType::Damage, false);
	}
	else if(GridMan && GridMan->GetAttachedFloor())
	{
		FVector PosDif = GridMan->GetGridCenter(CenterIndex) - GridMan->GetGridCenter(CurrentTargetGridIndex);
		GridMan->GetAttachedFloor()->SetProcMeshPosition(EISMType::Damage, PosDif);
	}
	CurrentTargetGridIndex = CenterIndex;
}

void AGGCharacter::UpdateCurrentGridIndex()
{
	GridManager* GridMan = CharacterManager::CharGridManager;
	
	if(GridMan && GridMan->GetAttachedFloor())
	{
		GridMan->SetGridState(CurrentGridIndex, EGridState::Empty);
		CurrentGridIndex = GridMan->WorldToGrid(GetActorLocation());
		GridMan->SetGridState(CurrentGridIndex, EGridState::Obstacle);
	}
}

void AGGCharacter::SetCustomDepth(bool Active, int StencilValue)
{
	USkeletalMeshComponent* Skeletal = this->GetMesh();
	if(Skeletal)
	{
		Skeletal->SetRenderCustomDepth(Active);
		Skeletal->SetCustomDepthStencilValue(StencilValue);
	}
}

void AGGCharacter::CastSkill(TArray<AGGCharacter*>& TargetCharacters)
{
	CharacterSkill* CurrentSkill = GetCurrentSkill();

	if(CurrentSkill== nullptr) return;

	//UpdateTrajectoryPath();

	SelectedTargetCharacters = TargetCharacters;
	
	FCharSkillFileDataTable* SkillFiles = &(CurrentSkill->GetSkillFiles());
	if(SkillFiles )
	{
		GridManager* GridMan = CharacterManager::CharGridManager;
		if(GridMan && GridMan->GetAttachedFloor())
		{
			FVector SelectedGridPos = GridMan->GetGridCenter(CurrentTargetGridIndex);
			FVector Dir =  SelectedGridPos - GetActorLocation();
			Dir.Z = 0;
			FRotator Rot = FRotationMatrix::MakeFromX(Dir).Rotator();

			ATimedEventManager::Rotate(this, Rot, 0.2f, GetWorld());

			GridMan->GetAttachedFloor()->ClearTrajectory();
		}

		FString TimerKey = CurrentSkill->GetSkillData().SkillName;
		FTimedEvent TimedEvent;
		TimedEvent.BindDynamic(this, &AGGCharacter::OnCastingSkillEnds);
		ATimedEventManager::CallEventWithDelay(this, TimerKey, TimedEvent, 10, GetWorld());
		
		SetStatus(ECharacterStatus::Casting);

		PlayCharacterMontage(SkillFiles->SkillMontage);
		bIsSkillMontagePlaying = true;

		if(SkillsCooldownMap.Contains(SkillFiles->SkillID))
		{
			FCooldownTimer* Timer = SkillsCooldownMap.Find(SkillFiles->SkillID);
			if(Timer)
			{
				Timer->RestartTimer();
				if(Timer->OnSkillCastedDelegate.IsBound())
				{
					Timer->OnSkillCastedDelegate.Execute();
				}
			}
		}
	}
}

void AGGCharacter::OnAttackHitsEnemies()
{
	Super::OnAttackHitsEnemies();

	CharacterSkill* CurrentSkill = GetCurrentSkill();

	if(CurrentSkill == nullptr) return;

	if(SelectedTargetCharacters.Num() > 0)
	{
		CurrentSkill->ApplyEffects(this, SelectedTargetCharacters);
		CurrentSkill->ApplyStatus(this, SelectedTargetCharacters);
	}
}

void AGGCharacter::OnAttackHitsEnemy(AActor* TargetToHit)
{
	Super::OnAttackHitsEnemy(TargetToHit);

	if(TargetToHit == nullptr) return;;
	
	CharacterSkill* CurrentSkill = GetCurrentSkill();

	if(CurrentSkill == nullptr) return;

	TArray<AGGCharacter*> Enemies;

	AGGCharacter* CharacterToHit = static_cast<AGGCharacter*>(TargetToHit);
	
	if(CharacterToHit && SelectedTargetCharacters.Contains(TargetToHit))
	{
		Enemies.Add(CharacterToHit);
		CurrentSkill->ApplyEffects(this, Enemies);
		CurrentSkill->ApplyStatus(this, Enemies);
	}
	
}

void AGGCharacter::OnDeath()
{
	Super::OnDeath();

	SetStatus(ECharacterStatus::Dead);
	PlayCharacterMontage(CharFile.GetRandomDeathMontage());
}

void AGGCharacter::OnCastingSkillEnds()
{
	Super::OnCastingSkillEnds();

	bIsSkillMontagePlaying = false;
	UE_LOG(LogTemp, Warning, TEXT("OnCastingSkillEnds "));
	CharacterSkill* CurrentSkill = GetCurrentSkill();

	if(CurrentSkill != nullptr)
	{
		FString TimerKey = Skills[CurrentSkillIndex]->GetSkillData().SkillName;
		ATimedEventManager::RemoveEventData(TimerKey, false);
	}

	ABattlePlayerController* PlayerController = Cast<ABattlePlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController != nullptr)
	{
		SetStatus(ECharacterStatus::Idle);
		PlayerController->ChangeStateTo(EControllerStateIndex::Movement);
	}
}

AActor* AGGCharacter::CreateProjectile(FName SocketName, bool bUseBoneRotation)
{
	CharacterSkill* CurrentSkill = GetCurrentSkill();

	if(CurrentSkill== nullptr) return nullptr;

	USkeletalMeshComponent* SkeletalMesh = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	if(/*SelectedTargetCharacters.Num() > 0 && */SkeletalMesh)
	{
		FCharSkillFileDataTable* SkillFiles = &(CurrentSkill->GetSkillFiles());
		FSkillData* SkillData = &(CurrentSkill->GetSkillData());
		if(SkillFiles && SkillData)
		{
			if(SkillFiles->EffectBP)
			{
				const FVector SocketLocation = SkeletalMesh->GetSocketLocation(SocketName);
				FRotator SocketRotation = SkeletalMesh->GetSocketRotation(SocketName);
				if(bUseBoneRotation == false)
				{
					SocketRotation = FRotator::ZeroRotator;
				}
				AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(SkillFiles->EffectBP, SocketLocation, SocketRotation, FActorSpawnParameters());
				if(Projectile)
				{
					Projectile->SelectedTargetCharacters = SelectedTargetCharacters;
					Projectile->OwnerCharacter = this;

					//FRotator Rot = Dir.Rotation();
					//Projectile->SetActorRotation(Rot);
					// FVector TargetLocation = SelectedTargetCharacters[0]->GetActorLocation();
					FVector TargetLocation = GetTargetTrajectoryLocation();
					Projectile->Angle = SkillData->LineOfSightAngle;
					Projectile->SetVelocityViaTarget(TargetLocation);
					Projectile->SetLifeSpan(10);
					
					return Projectile;
				}
			}
		}
	}
	
	return nullptr;
}

void AGGCharacter::CreateParticleOnTargetGrid()
{
	CharacterSkill* CurrentSkill = GetCurrentSkill();

	if(CurrentSkill== nullptr) return;
	

	FCharSkillFileDataTable* SkillFiles = &(CurrentSkill->GetSkillFiles());
	FSkillData* SkillData = &(CurrentSkill->GetSkillData());
	if(SkillFiles && SkillData)
	{
		GridManager* GridMan = CharacterManager::CharGridManager;
		if(SkillFiles->EffectBP  && GridMan && GridMan->GetAttachedFloor())
		{
			FVector TargetLocation = GridMan->GetGridCenter(CurrentTargetGridIndex);
			AActor* ParticleActor = GetWorld()->SpawnActor<AActor>(SkillFiles->EffectBP, TargetLocation, FRotator::ZeroRotator, FActorSpawnParameters());

			if(ParticleActor)
			{
				ParticleActor->SetLifeSpan(10);
			}
		}
	}
}

void AGGCharacter::CreateParticlesOnEveryEnemyInDamageable()
{
}

void AGGCharacter::ThrowProjectile(FName SocketName, bool bUseBoneRotation)
{
	AActor* Projectile = CreateProjectile(SocketName, bUseBoneRotation);
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
				AnimInstance->ClassType = CharFile.CharacterType;
			}
			
			// for (int i = 0; i < Weapons.Num(); ++i)
			// {
			// 	if(Weapons[i])
			// 	{
			// 		USkeletalMeshComponent* WpSkeletal = Cast<USkeletalMeshComponent>(Weapons[i]->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
			// 		if(WpSkeletal)
			// 		{
			// 			UCharacterAnimInstance* WpAnimInst = Cast<UCharacterAnimInstance>(WpSkeletal->GetAnimInstance());
			// 			if(WpAnimInst)
			// 			{
			// 				WpAnimInst->ClassType = CharFile.CharacterType;
			// 				if(AnimInstance)
			// 				{
			// 					if(AnimInstance->WeaponAnimInstances.Contains(WpAnimInst) == false)
			// 					{
			// 						UE_LOG(LogTemp, Warning, TEXT("ANIM INSTANCE %s"), *WpAnimInst->GetName());
			// 						AnimInstance->WeaponAnimInstances.Add(WpAnimInst);
			// 					}
			// 				}
			// 			}
			// 		}
			// 	}
			// }
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
	if(AnimInstance == nullptr || bIsSkillMontagePlaying/* || AnimInstance->Montage_IsPlaying(nullptr) == true*/) return;

	AnimInstance->PlayMontage(Montage);
}

CharacterSkill* AGGCharacter::GetCurrentSkill()
{
	if(CurrentSkillIndex >= Skills.Num()|| CurrentSkillIndex < 0 || Skills[CurrentSkillIndex] == nullptr)
	{
		return nullptr;
	}

	return Skills[CurrentSkillIndex];
}

TArray<CharacterSkill*>* AGGCharacter::GetSkills()
{
	return  &Skills;
}

bool AGGCharacter::SetCurrentSkillIfContains(int SkillId)
{
	for (int i = 0; i < Skills.Num(); ++i)
	{
		if(Skills[i])
		{
			FSkillData SkillData = Skills[i]->GetSkillData();
			if(SkillData.SkillID == SkillId)
			{
				CurrentSkillIndex = i;
				return true;
			}
		}
	}

	return false;
}

FVector AGGCharacter::GetTargetTrajectoryLocation()
{
	GridManager* GridMan = CharacterManager::CharGridManager;

	FVector TargetLocation = FVector::ZeroVector;
	if(GridMan && GridMan->GetAttachedFloor())
	{
		FVector GridCenterLoc = GridMan->GetGridCenter(CurrentTargetGridIndex);
		TargetLocation = GridCenterLoc  + FVector(0, 0, 5);

		AGGCharacter* TargetChar = GetCharacterAtTargetGridIndex();
		if(TargetChar)
		{
			TargetLocation = GridCenterLoc + FVector(0, 0, 130);
		}
	}
	return  TargetLocation;
}

FVector AGGCharacter::GetStartTrajectoryLocation()
{
	return  GetActorLocation() + FVector(0, 0, 20);
}

AGGCharacter* AGGCharacter::GetCharacterAtTargetGridIndex()
{
	GridManager* GridManager = CharacterManager::CharGridManager;
	if(GridManager)
	{
		return GridManager->GetCharacterByGridIndex(CurrentTargetGridIndex);
	}
	return nullptr;
}

bool AGGCharacter::CanTrajectoryBeShown()
{
	CharacterSkill* CurrentSkill = GetCurrentSkill();

	if(CurrentSkill)
	{
		FSkillData* SkillData = &(CurrentSkill->GetSkillData());
		if(SkillData)
		{
			return SkillData->ShowTrajectory;
		}
	}

	return  false;
}

void AGGCharacter::OnTurnEnds()
{
	for (auto It = SkillsCooldownMap.CreateIterator(); It; ++It)
	{
		It.Value().DecreaseTurn(1);
	}

	
}

void AGGCharacter::OnIndividualTurnBegins()
{
	StatusEffectManager::ApplyOnTurnBegins(this, &AppliedStatusEffects);
	UE_LOG(LogTemp, Warning, TEXT("STATUS EFFECTS %d"), AppliedStatusEffects.Num());
}

void AGGCharacter::OnIndividualTurnEnds()
{
	//StatusEffectManager::ApplyOnTurnEnds(this, &AppliedStatusEffects);
}


TMap<EStatusEffectType, struct FStatusEffectData>* AGGCharacter::GetAppliedStatusEffects()
{
	return &AppliedStatusEffects;
}

