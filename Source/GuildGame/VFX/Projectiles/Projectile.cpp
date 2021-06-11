// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GuildGame/Characters/GGCharacter.h"
#include "GuildGame/Managers/CharacterManager.h"
#include "GuildGame/Managers/GridManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	if(ProjectileMovementComponent)
	{
		ProjectileMovementComponent->ProjectileGravityScale = 0;
	}

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	if(SphereComponent)
	{
		FCollisionResponseContainer Response;
		Response.SetAllChannels(ECollisionResponse::ECR_Ignore);
		Response.SetResponse(ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
		Response.SetResponse(ECC_Pawn, ECollisionResponse::ECR_Overlap);
		Response.SetResponse(ECC_Destructible, ECollisionResponse::ECR_Overlap);

		SphereComponent->SetCollisionResponseToChannels(Response);
	}

	Speed = 1;
	PathIndex = 1;
	
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if(SphereComponent)
	{
		SphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &AProjectile::OnCollisionOverlapBegin);
	}
	// if(SelectedTargetCharacters.Num() <= 0)
	// {
	// 	if(OwnerCharacter)
	// 	{
	// 		//OwnerCharacter->OnAttackHitsEnemy();
	// 		OwnerCharacter->OnCastingSkillEnds();
	// 		Destroy();
	// 	}
	// }
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bProjectileDeactivated == true) return;
	
	if(bStartLaunch)
	{
		if(PathIndex < ProjectileResult.PathData.Num())
		{
			FVector NextNode = ProjectileResult.PathData[PathIndex].Location;
			FVector Direction = NextNode - GetActorLocation();

			
			FVector DirectionNormalized = Direction;
			DirectionNormalized.Normalize();
			
			FVector AddedVector = DirectionNormalized * Speed *  DeltaTime;
			if(AddedVector.Size() > Direction.Size())
			{
				AddedVector = Direction;
			}
			
			FVector NewLocation = GetActorLocation() + AddedVector;
			//FVector NewLocation = FMath::VInterpTo(GetActorLocation(), NextNode, DeltaTime, Speed);

			FRotator RawNewRotation = GetActorRotation();

			if(FVector::Dist(NewLocation, NextNode) > 0.05f)
			{
				RawNewRotation = UKismetMathLibrary::FindLookAtRotation( NewLocation, NextNode);
			}
		
			
			// FRotator NewRotation = FMath::Lerp(ActorRot, RawNewRotation, 0.5f);
			FRotator NewRotation = RawNewRotation;
			FTransform NewTransform;
			NewTransform.SetLocation(NewLocation);
			NewTransform.SetRotation(NewRotation.Quaternion());
			//NewTransform.SetRotation(GetActorRotation().Quaternion());
			NewTransform.SetScale3D(GetActorScale());

			SetActorTransform(NewTransform);

			Direction = NextNode - GetActorLocation();
			if(Direction.Size() <= 0.01f)
			{
				PathIndex++;
			}
		}
		else if(ProjectileResult.PathData.Num() > 1)
		{
			OnSkillHits(FVector::UpVector);
		}
	}

}

void AProjectile::SetVelocityViaTarget(FVector LocationOfTarget)
{
	FVector StartLocation = GetActorLocation();
    SetActorRotation(FRotator::ZeroRotator);

	GridManager* GridMan = CharacterManager::CharGridManager;

	if(GridMan)
	{
		bStartLaunch = GridMan->CanAttackTargetGrid(OwnerCharacter, ProjectileResult, StartLocation);

		if(ProjectileMovementComponent)
		{
			ProjectileMovementComponent->Velocity = FVector::ZeroVector;
		}
		if(bStartLaunch)
		{
			UE_LOG(LogTemp, Warning, TEXT("PROJECTILE NODES %d"), ProjectileResult.PathData.Num());
		}
		UE_LOG(LogTemp, Warning, TEXT("PROJECTILE bStartLaunch %d"), bStartLaunch);
	}

	// FVector VelocityOutput = GetVelocityVector(StartLocation, LocationOfTarget, Angle, GetWorld());
	// ProjectileMovementComponent->SetVelocityInLocalSpace(VelocityOutput);
}

FVector AProjectile::GetVelocityVector(FVector StartLocation, FVector TargetLocation, float Angle, UWorld* World)
{
	//  == Fixed Angle Projectile Velocity Calculation == 

	const float Gravity = World->GetGravityZ() * -1; // Gravity. (Must be a positive value)
	//const float Gravity = 980.f; // This is the same as the line above (unless your project settings have been changed)
	const float Theta = (Angle * PI / 180); // Launch angle in radians (40 being the launch angle in degrees)

	FVector Dir = TargetLocation - StartLocation; //direction
	float Sz = Dir.Z; // Height difference
	Dir.Z = 0; // Remove hight from direction
	float Sx = Dir.Size(); // Distance

	const float V = (Sx / cos(Theta)) * FMath::Sqrt((Gravity * 1) / (2 * (Sx * tan(Theta) - Sz)));
	FVector VelocityOutput = FVector(V*cos(Theta), 0, V*sin(Theta));
	const FRotator Newrot = UKismetMathLibrary::FindLookAtRotation( StartLocation, TargetLocation);
	VelocityOutput = VelocityOutput.RotateAngleAxis(Newrot.Yaw, FVector::ZAxisVector);

	return VelocityOutput;
}

bool AProjectile::TraceTrajectory(FVector StartPosition, FVector TargetPosition, float Angle, UWorld* World, FPredictProjectilePathResult& ProjectileResult, TArray<AActor*> ActorsToIgnore, TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToHit)
{
	FVector VelocityOutput = GetVelocityVector(StartPosition, TargetPosition, Angle, World);
	
	FPredictProjectilePathParams ProjectileParams;
	
	
	ProjectileParams.StartLocation = StartPosition;
	ProjectileParams.LaunchVelocity = VelocityOutput;
	ProjectileParams.ProjectileRadius = 2;
	ProjectileParams.MaxSimTime = 3;
	ProjectileParams.DrawDebugType = EDrawDebugTrace::None;
	ProjectileParams.bTraceWithCollision = true;
	ProjectileParams.bTraceWithChannel = false;

	ProjectileParams.ActorsToIgnore = ActorsToIgnore;
	
	ProjectileParams.ObjectTypes = ObjectTypesToHit;

	

	return UGameplayStatics::PredictProjectilePath(World, ProjectileParams, ProjectileResult);
}

void AProjectile::OnCollisionOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(bProjectileDeactivated == true) return;
	
	AGGCharacter* OverlapedChar = Cast<AGGCharacter>(OtherActor);
	if(OverlapedChar)
	{
		if(SelectedTargetCharacters.Contains(OverlapedChar))
		{
			if(OverlapedChar == OwnerCharacter)
			{
				// UE_LOG(LogTemp, Warning, TEXT("PROJECTILE OnCollisionOverlapBegin DESTROY"));
				OnSkillHits(SweepResult.Normal);
				//SetLife
			}
		}
	}
}

void AProjectile::OnSkillHits(FVector Normal)
{
	if(bProjectileDeactivated == true) return;
	OwnerCharacter->OnAttackHitsEnemies();
	OwnerCharacter->OnCastingSkillEnds();
	Destroy();
	FRotator HitRotation = UKismetMathLibrary::MakeRotFromX(Normal);
	UGameplayStatics::SpawnEmitterAtLocation(this, HitEffect, GetActorLocation(), HitRotation, true);
	bProjectileDeactivated = true;
}



